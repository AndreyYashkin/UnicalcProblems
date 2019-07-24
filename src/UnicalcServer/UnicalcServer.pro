QT       += core websockets
QT       -= gui

TARGET = UnicalcServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    UnicalcServer.cpp

HEADERS += \
    UnicalcServer.hpp

EXAMPLE_FILES += UnicalcServer.html

target.path = $$[QT_INSTALL_EXAMPLES]/websockets/UnicalcServer
INSTALLS += target

DISTFILES += \
    CHANGELOG.md \
    TODO
