TEMPLATE = app

QT += qml quick widgets webengine

include(3dparty/QmlVlc/QmlVlc.pri)
INCLUDEPATH += 3dparty

#== replace by cmake procedure
#INCLUDEPATH += ../vmf3-demo-player.vmf/include
#LIBS += -L../vmf3-demo-player.vmf/x64/lib -lvmfd
INCLUDEPATH += ../vmf-debug/install/include
LIBS += -L../vmf-debug/install/x64/lib -lvmfd
#==

SOURCES += main.cpp \
    MetadataProvider/MetadataProvider.cpp

HEADERS += \
    MetadataProvider/MetadataProvider.h

RESOURCES += qml.qrc

CONFIG += c++11

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

macx {
    LIBS += -L/Applications/VLC.app/Contents/MacOS/lib
}
