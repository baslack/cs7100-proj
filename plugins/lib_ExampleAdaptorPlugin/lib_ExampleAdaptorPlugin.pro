TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += ../../app
INCLUDEPATH    += ../../plugins
TARGET          = $$qtLibraryTarget(_ExampleAdaptorPlugin)
DESTDIR         = ../../app/plugins

HEADERS += \
    exampleadaptorplugin.h \
    ../adaptorplugin.h \
    ../../app/example.h

SOURCES += \
    exampleadaptorplugin.cpp \
    ../adaptorplugin.cpp \
    ../../app/example.cpp

CONFIG += qt debug

