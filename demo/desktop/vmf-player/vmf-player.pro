TEMPLATE = app

QT += qml quick widgets webengine

include(3dparty/QmlVlc/QmlVlc.pri)
INCLUDEPATH += 3dparty

#== TODO replace by cmake procedure
isEmpty(VMF_DIR) {
    message("'VMF_DIR' variable is empty, the default value '../vmf-build/install' is used")
    VMF_DIR = ../vmf-build/install
}
INCLUDEPATH += $$VMF_DIR/include ../../../$$VMF_DIR/include

LIBS += -L$$VMF_DIR/x64/lib -L$$VMF_DIR/x64/vc12/lib
LIBS += -L../../../$$VMF_DIR/x64/lib -L../../../$$VMF_DIR/x64/vc12/lib

CONFIG(debug, debug|release) {
    LIBS += -lvmfd
} else {
    LIBS += -lvmf
}
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
