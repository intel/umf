#ifndef METADATAPROVIDER_H
#define METADATAPROVIDER_H

#include <vmf/vmf.hpp>

#include <QObject>
#include <QPointF>
#include <QString>
#include <QQmlListProperty>

#include <QtConcurrent/QtConcurrent>

#include <atomic>
#include <mutex>
#include <thread>

class Location : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double latitude READ latitude WRITE setLatitude)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude)
//    Q_PROPERTY(double altitude READ altitude WRITE setAltitude)
//    Q_PROPERTY(double accuracy READ accuracy WRITE setAccuracy)
//    Q_PROPERTY(double speed READ speed WRITE setSpeed)

public:
    explicit Location(QObject* parent = 0) :
        QObject(parent), m_latitude(0.0), m_longitude(0.0)
    { }

    Location(const Location& l) :
        QObject(l.parent()), m_latitude(l.m_latitude), m_longitude(l.m_longitude)
    { }

    ~Location()
    { }

    double latitude() const { return m_latitude; }
    void setLatitude(double latitude) { m_latitude = latitude; }

    double longitude() const { return m_longitude; }
    void setLongitude(double longitude) { m_longitude = longitude; }

//    double altitude() const { return m_altitude; }
//    void setAltitude(double altitude) { m_altitude = altitude; }

//    double accuracy() const { return m_accuracy; }
//    void setAccuracy(double accuracy) { m_accuracy = accuracy; }

//    double speed() const { return m_speed; }
//    void setSpeed(double speed) { m_speed = speed; }

private:
    double m_latitude;
    double m_longitude;
//    double m_altitude;
//    double m_accuracy;
//    double m_speed;
};


Q_DECLARE_METATYPE(Location)
Q_DECLARE_METATYPE(QQmlListProperty<Location>)


//class for passing info to GUI
class WrappingInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString compressionID READ compressionID WRITE setCompressionID)
    Q_PROPERTY(QString passphrase READ passphrase WRITE setPassphrase)
    Q_PROPERTY(QString format READ format WRITE setFormat)

public:

    explicit WrappingInfo(QObject* parent = 0) :
        QObject(parent), m_compressionID(""), m_passphrase(""), m_format("")
    { }

    WrappingInfo(const WrappingInfo& wi) :
        QObject(wi.parent()),
        m_compressionID(wi.m_compressionID),
        m_passphrase(wi.m_passphrase),
        m_format(wi.m_format)
    { }

    WrappingInfo& operator=(const WrappingInfo& wi)
    {
        //QObject::operator =(wi);
        m_compressionID = wi.m_compressionID;
        m_passphrase = wi.m_passphrase;
        m_format = wi.m_format;
        return *this;
    }

    ~WrappingInfo()
    { }

    QString compressionID() const { return m_compressionID; }
    void setCompressionID(const QString& id) { m_compressionID = id; }

    QString passphrase() const { return m_passphrase; }
    void setPassphrase(const QString& pwd) { m_passphrase = pwd; }

    QString format() const { return m_format; }
    void setFormat(const QString& f) { m_format = f; }

private:
    QString m_compressionID;
    QString m_passphrase;
    QString m_format;
};

Q_DECLARE_METATYPE(WrappingInfo)


//class for passing info to GUI
class StatInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int count READ count WRITE setCount)
    Q_PROPERTY(double minLat READ minLat WRITE setMinLat)
    Q_PROPERTY(double avgLat READ avgLat WRITE setAvgLat)
    Q_PROPERTY(double lastLat READ lastLat WRITE setLastLat)

public:

    explicit StatInfo(QObject* parent = 0) :
        QObject(parent),
        m_count(0),
        m_minLat(0),
        m_avgLat(0),
        m_lastLat(0)
    { }

    StatInfo(const StatInfo& si) :
        QObject(si.parent()),
        m_count(si.m_count),
        m_minLat(si.m_minLat),
        m_avgLat(si.m_avgLat),
        m_lastLat(si.m_lastLat)
    { }

    StatInfo& operator=(const StatInfo& si)
    {
        //QObject::operator =(si);
        m_count = si.m_count;
        m_minLat = si.m_minLat;
        m_avgLat = si.m_avgLat;
        m_lastLat = si.m_lastLat;
        return *this;
    }

    ~StatInfo()
    { }

    int count() const { return m_count; }
    void setCount(int c) { m_count = c; }

    double minLat() const { return m_minLat; }
    void setMinLat(double v) { m_minLat = v; }

    double avgLat() const { return m_avgLat; }
    void setAvgLat(double v) { m_avgLat = v; }

    double lastLat() const { return m_lastLat; }
    void setLastLat(double v) { m_lastLat = v; }

private:
    int m_count;
    double m_minLat;
    double m_avgLat;
    double m_lastLat;
};

Q_DECLARE_METATYPE(StatInfo)


class MetadataProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QQmlListProperty<Location> locations READ locations)
    Q_PROPERTY(WrappingInfo* wrappingInfo READ wrappingInfo)
    Q_PROPERTY(StatInfo* statInfo READ statInfo)
    Q_PROPERTY(QString deviceId READ deviceId)
    Q_PROPERTY(double lastTimestamp READ lastTimestamp)

public:
    explicit MetadataProvider(QObject *parent = 0);
    ~MetadataProvider();

    QString address();
    void setAddress(const QString& address);

    QQmlListProperty<Location> locations();

    WrappingInfo* wrappingInfo();

    StatInfo* statInfo();

    QString deviceId();

    double lastTimestamp();

signals:
    void addressChanged();
    void segmentAdded();
    void schemaAdded();
    void metadataAdded();

public slots:
    void start();
    void stop();

private:
    QString getAddress() const;
    bool putAddress(const QString& address);
    int m_ip;
    int m_port;

    bool connect();
    void disconnect();
    void execute();

    void updateLocations();
    static double getFieldValue(std::shared_ptr<vmf::Metadata> md, const std::string& name);
    void setWrappingStatus(std::shared_ptr<vmf::Format> f,
                           std::shared_ptr<vmf::Encryptor> e,
                           std::string buf);

    QFuture<void> m_worker;
    std::atomic<bool> m_working;
    std::atomic<bool> m_exiting;
    int m_sock;
    bool m_useXml;

    vmf::MetadataStream m_ms;
    QList<Location*> m_locations;
    WrappingInfo* m_wrappingInfo;
    StatInfo* m_statInfo;
    QString m_deviceId;
    std::atomic<double> m_lastTimestamp;
    mutable std::mutex m_lock;

    class ConnectionLock;
};

#endif // METADATAPROVIDER_H
