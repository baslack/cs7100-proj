#-------------------------------------------------
#
# Project created by QtCreator 2018-07-21T13:44:21
#
#-------------------------------------------------

QT       += testlib

TARGET = Tests

CONFIG   += console

CONFIG   -= app_bundle

TEMPLATE = app

include(tests.pri)

DEFINES += SRCDIR=\\\"$$PWD/\\\"

DISTFILES += \
    tests.pri
