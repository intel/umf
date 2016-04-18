#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QmlVlc.h>
#include <QmlVlc/QmlVlcConfig.h>

#include <QtWebEngine/qtwebengineglobal.h>

#include <MetadataProvider/MetadataProvider.h>

#ifdef WIN32
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#endif

int main(int argc, char *argv[])
{
    RegisterQmlVlc();
    QmlVlcConfig& config = QmlVlcConfig::instance();
    config.enableDebug( false );

    qmlRegisterType<MetadataProvider>("vmf3.demo.metadata", 1, 0, "MetadataProvider");

    qmlRegisterType<Location>("vmf3.demo.metadata", 1, 0, "Location");
    qmlRegisterType<WrappingInfo>("vmf3.demo.metadata", 1, 0, "WrappingInfo");
    qmlRegisterType<StatInfo>("vmf3.demo.metadata", 1, 0, "StatInfo");

#ifdef WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2),&wsa);
#endif

    QApplication app(argc, argv);

    QtWebEngine::initialize();

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

