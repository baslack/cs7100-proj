TEMPLATE         = lib
CONFIG          += plugin
QT              += widgets
INCLUDEPATH     += \
    ../../app \
    ../../plugins
TARGET           = $$qtLibraryTarget(_Example2AdaptorPlugin)
DESTDIR          = ../../app/plugins

HEADERS += lib_example2adaptorplugin.h \
    ../adaptorplugin.cpp \
    ../../app/example.cpp

SOURCES += lib_example2adaptorplugin.cpp \
    ../adaptorplugin.cpp \
    ../../app/example2.cpp

CONFIG += qt debug
