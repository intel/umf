#include "MetadataProvider.h"

#if defined(WIN32)
# include <stdio.h>
# include <Ws2tcpip.h>
#else //if defined(__linux__)
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
#endif

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
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

static const size_t bufSize = 1 << 20;

static const std::string streamCompressionId = "com.intel.vmf.compressor.zlib";
static const std::string streamPassphrase = "VMF Demo passphrase!";

#if defined(USE_NATIVE_ENDIAN)
# define zzntohl(_sz) (_sz)
#else
# define zzntohl(_sz) ntohl(_sz)
#endif

static ssize_t sendMessage(int fd, const char* buf, size_t msgSize)
{
    return ::send(fd, buf, (int)msgSize, 0);
}

static ssize_t receiveMessage(int fd, char* buf, size_t bufSize, bool doWait = false)
{
    const int  flags = (doWait ? MSG_WAITALL : 0);
    uint32_t sz = 0;
    ssize_t size = ::recv(fd, (char*)&sz, 4, flags);
    if ((size == 4) && ((sz = zzntohl(sz)) < bufSize))
    {
        size = ::recv(fd, buf, sz, flags);
        if (size == sz)
        {
            buf[sz] = 0;
        }
    }
    return size;
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
    , m_useXml(true)
    , m_wrappingInfo(new WrappingInfo())
    , m_statInfo(new StatInfo())
    , m_deviceId("")
    , m_lastTimestamp(0)
{
    vmf::Log::setVerbosityLevel(vmf::LogLevel::LOG_ERROR);
}

MetadataProvider::~MetadataProvider()
{
    stop();
    delete m_wrappingInfo;
    delete m_statInfo;
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
#ifdef WIN32
        if (inet_pton(AF_INET, ipStr.toStdString().c_str(), &in))
        #else
        if (inet_aton(ipStr.toStdString().c_str(), &in))
#endif
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
        m_worker = QtConcurrent::run(this, &MetadataProvider::execute);
    }
}

void MetadataProvider::stop()
{
    std::cerr << "*** MetadataProvider::stop()" << std::endl;
    if (m_working)
    {
        std::cerr << "*** MetadataProvider::stop() : stopping worker..." << std::endl;
        m_exiting = true;
        m_worker.waitForFinished();
        std::cerr << "*** MetadataProvider::stop() : worker stopped" << std::endl;
        m_working = false;
        m_ms.clear();
        m_locations.clear();
        std::cerr << "*** MetadataProvider::stop() : disconnecting..." << std::endl;
        disconnect();
    }
}

bool MetadataProvider::connect()
{
    const int domain = AF_INET;
    const int type = SOCK_STREAM;
    const int protocol = 0;

    m_sock = ::socket(domain, type, protocol);

    //limit timeout to 5 secs
#ifdef WIN32
    DWORD ms = 5000;
    const char * ptv = (const char *)&ms;
    int tvSize = sizeof(ms);
#else
    timeval tv = {5, 0};
    timeval * ptv = &tv;
    int tvSize = sizeof(tv);
#endif
    ::setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, ptv, tvSize);

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

                // XML or JSON/OK
                size = receiveMessageRaw(m_sock, buf, sizeof(buf));
                bool isXml  = (size == 3) && (buf[0] == 'X') && (buf[1] == 'M') && (buf[2] == 'L');
                bool isJson = (size == 4) && (buf[0] == 'J') && (buf[1] == 'S') && (buf[2] == 'O') && (buf[3] == 'N');
                if (isXml || isJson)
                {
                    m_useXml = isXml;

                    buf[0] = 'O';
                    buf[1] = 'K';
                    size = sendMessage(m_sock, buf, 2);

                    return true;
                }
            }
        }
#ifdef WIN32
        ::closesocket(m_sock);
#else
        ::close(m_sock);
#endif
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

#ifdef WIN32
        ::closesocket(m_sock);
#else
        ::close(m_sock);
#endif
        m_sock = -1;

        std::cerr << "Socket closed" << std::endl;
    }
}


void MetadataProvider::setWrappingStatus(std::shared_ptr<vmf::Format> f,
                                         std::shared_ptr<vmf::Encryptor> e,
                                         std::string buf)
{
    //this parser should be transparent for encryption
    //it shows existence (or absence) of compression schema in input data
    vmf::FormatEncrypted compressionDetector(f, e);

    std::vector<vmf::MetadataInternal> tmpMetadata;
    std::vector<std::shared_ptr<vmf::MetadataSchema>> tmpSchemas;
    std::vector<std::shared_ptr<vmf::MetadataStream::VideoSegment>> tmpSegments;
    std::vector<std::shared_ptr<vmf::Stat>> tmpStats;
    vmf::Format::AttribMap tmpAttribs;
    vmf::Format::ParseCounters tmpCounter;

    std::string toSetCompressionId, toSetPassphrase;
    //check encryption status
    tmpCounter = f->parse(buf, tmpMetadata, tmpSchemas, tmpSegments,
                          tmpStats, tmpAttribs);
    if(tmpCounter.schemas && tmpSchemas[0]->getName() == "com.intel.vmf.encrypted-metadata")
    {
        toSetPassphrase = streamPassphrase;
    }
    //check compression status
    tmpSchemas.clear();
    tmpCounter = compressionDetector.parse(buf, tmpMetadata, tmpSchemas,
                                           tmpSegments, tmpStats, tmpAttribs);
    if(tmpCounter.schemas && tmpSchemas[0]->getName() == "com.intel.vmf.compressed-metadata")
    {
        toSetCompressionId = streamCompressionId;
    }

    std::unique_lock< std::mutex > lock( m_lock );
    m_wrappingInfo->setCompressionID(QString::fromStdString(toSetCompressionId));
    m_wrappingInfo->setPassphrase(QString::fromStdString(toSetPassphrase));
    m_wrappingInfo->setFormat(QString::fromStdString(m_useXml ? "XML" : "JSON"));
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
            std::shared_ptr<vmf::Format> f;
            if(m_useXml)
            {
                f = std::make_shared<vmf::FormatXML>();
            }
            else
            {
                f = std::make_shared<vmf::FormatJSON>();
            }

            std::shared_ptr<vmf::Format> cf = std::make_shared<vmf::FormatCompressed>(f, streamCompressionId);
            std::shared_ptr<vmf::Encryptor> e = std::make_shared<vmf::EncryptorDefault>(streamPassphrase);
            vmf::FormatEncrypted parser(cf, e);

            std::vector<vmf::MetadataInternal> metadata;
            std::vector<std::shared_ptr<vmf::MetadataSchema>> schemas;
            std::vector<std::shared_ptr<vmf::MetadataStream::VideoSegment>> segments;
            std::vector<std::shared_ptr<vmf::Stat>> stats;
            vmf::Format::AttribMap attribs;
            vmf::Format::ParseCounters c;

            char buf[bufSize];

            if (!m_exiting)
            {
                ssize_t size = receiveMessage(m_sock, buf, sizeof(buf), true);
                if (size > 0)
                {
                    //get actual compression and encryption status
                    setWrappingStatus(f, e, std::string(buf));

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
                    //get actual compression and encryption status
                    setWrappingStatus(f, e, std::string(buf));

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
            try
            {
                std::shared_ptr<vmf::Stat> stat = m_ms.getStat(statName);
            }
            catch(vmf::NotFoundException&)
            {
                //add it to the stream if it does not exist yet
                std::vector< vmf::StatField > statFields;
                statFields.emplace_back(countStatName, schemaName, descName, latFieldName,
                                        vmf::StatOpFactory::builtinName(vmf::StatOpFactory::BuiltinOp::Count));
                statFields.emplace_back(minStatName, schemaName, descName, latFieldName,
                                        vmf::StatOpFactory::builtinName(vmf::StatOpFactory::BuiltinOp::Min));
                statFields.emplace_back(avgStatName, schemaName, descName, latFieldName,
                                        vmf::StatOpFactory::builtinName(vmf::StatOpFactory::BuiltinOp::Average));
                statFields.emplace_back(lastStatName, schemaName, descName, latFieldName,
                                        vmf::StatOpFactory::builtinName(vmf::StatOpFactory::BuiltinOp::Last));
                m_ms.addStat(std::make_shared<vmf::Stat>(statName, statFields, vmf::Stat::UpdateMode::Manual));
            }

            while (!m_exiting)
            {
                ssize_t size = receiveMessage(m_sock, buf, sizeof(buf), true);
                if (size > 0)
                {
                    std::cerr << std::string(buf) << std::endl;

                    //get actual compression and encryption status
                    setWrappingStatus(f, e, std::string(buf));

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

double MetadataProvider::lastTimestamp()
{
    return m_lastTimestamp;
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
    catch (const std::exception&)
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

    QList<Location*> newLocations;
    for(const auto& md : ms)
    {
        Location* loc = new Location();
        loc->setLatitude( getFieldValue(md, latFieldName));
        loc->setLongitude(getFieldValue(md, lngFieldName));
        newLocations.append(loc);
    }

    m_locations = newLocations;

    //update statistics: doRescan + doWait
    std::shared_ptr<vmf::Stat> stat = m_ms.getStat(statName);
    stat->update(true);

    //grab statistics
    m_statInfo->setCount((vmf::vmf_integer)stat->getField(countStatName).getValue());
    m_statInfo->setMinLat(stat->getField(minStatName).getValue());
    m_statInfo->setAvgLat(stat->getField(avgStatName).getValue());
    m_statInfo->setLastLat(stat->getField(lastStatName).getValue());

    //set last timestamp
    m_lastTimestamp = ms.back()->getTime();

    std::cerr << "*** MetadataProvider::updateLocations() ***" << std::endl;
}

