#-------------------------------------------------
#
# Project created by QtCreator 2018-12-31T13:18:28
#
#-------------------------------------------------

QT += core widgets websockets

TARGET = UnicalcClient
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    UCProblem.cpp \
    UnicalcClient.cpp \
    ConnectionDialog.cpp \
    ForUniAndVal.cpp \
    Global.cpp \
    OutputWindow.cpp \
    UniFileViewer.cpp

HEADERS += \
    UCProblem.hpp \
    UCProblemsFactory.hpp \
    UnicalcClient.hpp \
    ConnectionDialog.hpp \
    Example.hpp \
    ForUniAndVal.hpp \
    Global.hpp \
    OutputWindow.hpp \
    UniFileViewer.hpp
unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    UCMainWindow.ui \
    ConnectionDialog.ui \
    OutputWindow.ui \
    UniFileViewer.ui

DISTFILES += \
    CHANGELOG.md \
    TODO

DEFINES += UNICALCCLIENTLIB_LIBRARY
#DEFINES += WEBVERSION #См. QTBUG-73678

RESOURCES += \
    logo.qrc \
    icons.qrc

#contains(DEFINES, WEBVERSION)
#{
#    RESOURCES +=  icons.qrc #эти файлы используются только в web версии
#}
