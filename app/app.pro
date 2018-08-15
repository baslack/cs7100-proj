TEMPLATE = app

TARGET = funcVis

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(app.pri)

DISTFILES += \
    app.pri

QMAKE_POST_LINK += $$quote(cp -f $${PWD}/*.glsl $${OUT_PWD})
