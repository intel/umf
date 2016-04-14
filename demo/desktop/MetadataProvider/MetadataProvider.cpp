#include "MetadataProvider.h"

#if defined(WIN32)
# include <winsock2.h>
# include <stdio.h>
# include <windows.h>
//# pragma comment(lib, "ws2_32.lib")
#else //if defined(__linux__)
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
#endif

#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

//#define USE_NATIVE_ENDIAN
#define USE_SIZES_ON_HANDSHAKE

static const std::string schemaName = vmf::MetadataSchema::getStdSchemaName();
static const std::string descName = "location";
static const std::string latFieldName = "latitude";
static const std::string lngFieldName = "longitude";
static const std::string countStatName = "count";
static const std::string minStatName = "minLat";
static const std::string avgStatName = "avgLat";
static const std::string lastStatName = "lastLat";
static const std::string statName = "stat";

#if defined(USE_NATIVE_ENDIAN)
# define zzntohl(_sz) (_sz)
#else
# define zzntohl(_sz) ntohl(_sz)
#endif

static ssize_t sendMessage(int fd, const char* buf, size_t msgSize)
{
    return ::send(fd, (void*) buf, msgSize, 0);
}

static ssize_t receiveMessage(int fd, char* buf, size_t bufSize, bool doWait = false)
{
    const int  flags = (doWait ? MSG_WAITALL : 0);
    uint32_t sz = 0;
    ssize_t size = ::recv(fd, (void*) &sz, 4, flags);
    if ((size == 4) && ((sz = zzntohl(sz)) < bufSize))
    {
        size = ::recv(fd, (void*) buf, sz, flags);
        if (size == sz)
        {
            buf[sz] = 0;
            return size;
        }
    }
    return (ssize_t)-1;
}

static ssize_t receiveMessageRaw(int fd, char* buf, size_t msgSize)
{
#if defined(USE_SIZES_ON_HANDSHAKE)
    return receiveMessage(fd, buf, msgSize, true);
#else
    return ::recv(fd, (void*) buf, msgSize, true);
#endif
}

class MetadataProvider::ConnectionLock
{
public:
    ConnectionLock(MetadataProvider* mp)
        : m_this(mp)
        , m_success(false)
        { m_success = m_this->connect(); }
    ~ConnectionLock()
        { m_this->disconnect(); }
    bool isSuccessful() const
        { return m_success; }
private:
    MetadataProvider* m_this;
    bool m_success;
};

MetadataProvider::MetadataProvider(QObject *parent)
    : QObject(parent)
    , m_ip(0)
    , m_port(0)
    , m_working(false)
    , m_exiting(false)
    , m_sock(-1)
    , m_wrappingInfo(new WrappingInfo())
    , m_statInfo(new StatInfo())
    , m_deviceId("")
{
    vmf::Log::setVerbosityLevel(vmf::LogLevel::LOG_ERROR);
}

MetadataProvider::~MetadataProvider()
{
    delete m_wrappingInfo;
    delete m_statInfo;
    stop();
}

QString MetadataProvider::address()
{
    return getAddress();
}

void MetadataProvider::setAddress(const QString& address)
{
    if (putAddress(address))
        emit addressChanged();
}

QString MetadataProvider::getAddress() const
{
    struct in_addr in;
    in.s_addr = m_ip;
    QString ipStr(inet_ntoa(in));

    std::stringstream ss;
    ss << ntohs(m_port);
    QString portStr(ss.str().c_str());

    return ipStr + ":" + portStr;
}

bool MetadataProvider::putAddress(const QString& address)
{
    QStringList list = address.split(":");
    if (list.size() == 2)
    {
        const QString& ipStr = list.first();
        const QString& portStr = list.last();

        struct in_addr in;
        if (inet_aton(ipStr.toStdString().c_str(), &in))
        {
            bool ok = false;
            int port = portStr.toInt(&ok, 10);
            if (ok)
            {
                port = htons(port);
                if (((int)in.s_addr != m_ip) || (port != m_port))
                {
                    m_ip = in.s_addr;
                    m_port = port;
                    return true;
                }
                return false;
            }
        }
    }
    throw std::runtime_error("syntax error in ip:port address string");
}

void MetadataProvider::start()
{
    std::cerr << "*** MetadataProvider::start()" << std::endl;
    if (!m_working)
    {
        std::cerr << "*** MetadataProvider::start() : start worker" << std::endl;
        m_exiting = false;
        m_worker = std::thread(&MetadataProvider::execute, this);
    }
}

void MetadataProvider::stop()
{
    std::cerr << "*** MetadataProvider::stop()" << std::endl;
    if (m_working)
    {
        std::cerr << "*** MetadataProvider::stop() : stop worker" << std::endl;
        m_exiting = true;
        m_worker.join();
        m_working = false;
        m_ms.clear();
        m_locations.clear();
        disconnect();
    }
}

bool MetadataProvider::connect()
{
    const int domain = AF_INET;
    const int type = SOCK_STREAM;
    const int protocol = 0;

    m_sock = ::socket(domain, type, protocol);
    if (m_sock >= 0)
    {
        struct sockaddr_in server;

        memset(&server, 0, sizeof(server));
        server.sin_family = domain;
        server.sin_addr.s_addr = m_ip; //inet_addr(ip.toStdString().c_str());
        server.sin_port = m_port; //htons(port);

        int status = ::connect(m_sock, (const struct sockaddr*) &server, sizeof(struct sockaddr));
        if (status >= 0)
        {
            char buf[40000];

            // VMF/VMF
            ssize_t size = receiveMessageRaw(m_sock, buf, sizeof(buf));
            if ((size == 3) && (buf[0] == 'V') && (buf[1] == 'M') && (buf[2] == 'F'))
            {
                size = sendMessage(m_sock, buf, 3);

                // XML/OK
                size = receiveMessageRaw(m_sock, buf, sizeof(buf));
                if ((size == 3) && (buf[0] == 'X') && (buf[1] == 'M') && (buf[2] == 'L'))
                {
                    buf[0] = 'O';
                    buf[1] = 'K';
                    size = sendMessage(m_sock, buf, 2);

                    return true;
                }
            }
        }

        ::close(m_sock);
        m_sock = -1;
    }
    return false;
}

void MetadataProvider::disconnect()
{
    if (m_sock >= 0)
    {
        char buf[4];

        buf[0] = 'B';
        buf[1] = 'Y';
        buf[2] = 'E';
        sendMessage(m_sock, buf, 3);

        ::close(m_sock);
        m_sock = -1;
    }
}

void MetadataProvider::execute()
{
    std::cerr << "*** MetadataProvider::execute()" << std::endl;
    try
    {
        m_working = true;

        std::cerr << "*** MetadataProvider::execute() : trying to connect" << std::endl;
        ConnectionLock connection(this);
        std::cerr << "*** MetadataProvider::execute() : connect : " << (connection.isSuccessful() ? "SUCC" : "FAIL") << std::endl;
        if (connection.isSuccessful())
        {
            std::shared_ptr<vmf::Format> f = std::make_shared<vmf::FormatXML>();

            //actual compressor ID will be recognized at parse() call, but should be the following
            //TODO: specify actual Encryptor and passphrase
            {
                std::unique_lock< std::mutex > lock( m_lock );
                m_wrappingInfo->setCompressionID("com.intel.vmf.compressor.zlib");
                m_wrappingInfo->setPassphrase("");
            }

            f = std::make_shared<vmf::FormatCompressed>(f, m_wrappingInfo->compressionID().toStdString());

            vmf::FormatEncrypted parser(f, nullptr);

            std::vector<vmf::MetadataInternal> metadata;
            std::vector<std::shared_ptr<vmf::MetadataSchema>> schemas;
            std::vector<std::shared_ptr<vmf::MetadataStream::VideoSegment>> segments;
            std::vector< vmf::Stat > stats;
            vmf::Format::AttribMap attribs;
            vmf::Format::ParseCounters c;

            char buf[10240];

            if (!m_exiting)
            {
                ssize_t size = receiveMessage(m_sock, buf, sizeof(buf), true);
                if (size > 0)
                {
                    c = parser.parse(std::string(buf), metadata, schemas, segments, stats, attribs);
                    if (!(c.segments > 0))
                        throw std::runtime_error("expected video segment(s) not sent by server");
                    for (auto segment : segments)
                    {
                        std::unique_lock< std::mutex > lock( m_lock );
                        if(m_ms.getAllVideoSegments().size() == 0)
                        {
                            m_ms.addVideoSegment(segment);
                        }
                    }
                    m_deviceId = QString::fromStdString(m_ms.getAllVideoSegments()[0]->getTitle());
                    emit segmentAdded();
                }
            }

            if (!m_exiting)
            {
                ssize_t size = receiveMessage(m_sock, buf, sizeof(buf), true);
                if (size > 0)
                {
                    c = parser.parse(std::string(buf), metadata, schemas, segments, stats, attribs);
                    if (!(c.schemas > 0))
                        throw std::runtime_error("expected video schema(s) not sent by server");
                    for (auto schema : schemas)
                    {
                        std::unique_lock< std::mutex > lock( m_lock );
                        if(!m_ms.getSchema(schema->getName()))
                        {
                            m_ms.addSchema(schema);
                        }
                    }
                    emit schemaAdded();
                }
            }

            //set up statistics object
            std::vector< vmf::StatField > statFields;
            statFields.emplace_back(countStatName, schemaName, descName, latFieldName,
                                    vmf::StatOpFactory::builtinName(vmf::StatOpFactory::BuiltinOp::Count));
            statFields.emplace_back(minStatName, schemaName, descName, latFieldName,
                                    vmf::StatOpFactory::builtinName(vmf::StatOpFactory::BuiltinOp::Min));
            statFields.emplace_back(avgStatName, schemaName, descName, latFieldName,
                                    vmf::StatOpFactory::builtinName(vmf::StatOpFactory::BuiltinOp::Average));
            statFields.emplace_back(lastStatName, schemaName, descName, latFieldName,
                                    vmf::StatOpFactory::builtinName(vmf::StatOpFactory::BuiltinOp::Last));
            m_ms.addStat(vmf::Stat(statName, statFields, vmf::Stat::UpdateMode::Manual));

            while (!m_exiting)
            {
                ssize_t size = receiveMessage(m_sock, buf, sizeof(buf), true);
                if (size > 0)
                {
                    std::cerr << std::string(buf) << std::endl;

                    metadata.clear();
                    c = parser.parse(std::string(buf), metadata, schemas, segments, stats, attribs);
                    if (!(c.metadata > 0))
                        throw std::runtime_error("expected metadata not sent by server");
                    int num = 0;
                    for (auto md : metadata)
                    {
                        std::unique_lock< std::mutex > lock( m_lock );
                        //md.id = vmf::INVALID_ID;
                        m_ms.add(md);
                        ++num;
                    }
                    updateLocations();
                    std::cerr << "*** MetadataProvider::execute() : points per message = " << num << std::endl;
                    if(!m_exiting)
                    {
                        emit metadataAdded();
                    }
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "[MetadataProvider] EXCEPTION: " << e.what() << std::endl;
    }
    std::cerr << "*** MetadataProvider::execute() ***" << std::endl;
}


QQmlListProperty<Location> MetadataProvider::locations()
{
    std::unique_lock< std::mutex > lock( m_lock );
    return QQmlListProperty<Location>(this, m_locations);
}

WrappingInfo* MetadataProvider::wrappingInfo()
{
    std::unique_lock< std::mutex > lock( m_lock );
    return m_wrappingInfo;
}

StatInfo* MetadataProvider::statInfo()
{
    std::unique_lock< std::mutex > lock( m_lock );
    return m_statInfo;
}

QString MetadataProvider::deviceId()
{
    std::unique_lock< std::mutex > lock( m_lock );
    return m_deviceId;
}

double MetadataProvider::getFieldValue(std::shared_ptr<vmf::Metadata> md, const std::string& name)
{
    try
    {
        vmf::Variant fv = md->getFieldValue(name);
        if (fv.getType() == vmf::FieldValue::type_real)
            return fv.get_real();
        else
            return 0;
    }
    catch (const std::exception& e)
    {
        return 0;
    }
}

void MetadataProvider::updateLocations()
{
    std::unique_lock< std::mutex > lock( m_lock );
    std::cerr << "*** MetadataProvider::updateLocations()" << std::endl;

    vmf::MetadataSet ms = m_ms.getAll();

    std::sort(ms.begin(), ms.end(), [](std::shared_ptr<vmf::Metadata> a, std::shared_ptr<vmf::Metadata> b) -> bool{
        return (a->getId() < b->getId());
    });

    m_locations.clear();
    for(std::shared_ptr<vmf::Metadata> md : ms)
    {
        Location* loc = new Location();
        loc->setLatitude( getFieldValue(md, latFieldName));
        loc->setLongitude(getFieldValue(md, lngFieldName));
        m_locations.append(loc);
    }

    //update statistics: doRescan + doWait
    vmf::Stat& stat = m_ms.getStat(statName);
    stat.update(true, true);

    //grab statistics
    m_statInfo->setCount((vmf::vmf_integer)stat.getField(countStatName).getValue());
    m_statInfo->setMinLat(stat.getField(minStatName).getValue());
    m_statInfo->setAvgLat(stat.getField(avgStatName).getValue());
    m_statInfo->setLastLat(stat.getField(lastStatName).getValue());
}

