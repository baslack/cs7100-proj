SOURCES += \
    ../app/main.cpp \
    ../app/ui.cpp \
    ../app/gl.cpp \
#    ../app/adaptor.cpp \
    ../app/example.cpp \
#    ../app/exampleadaptor.cpp \
    ../app/paramwidget.cpp \
    $$PWD/example2.cpp \
    $$PWD/functionaction.cpp \
    $$PWD/cube.cpp \
    $$PWD/mesh.cpp \
    $$PWD/primatives.cpp

HEADERS += \
    ../app/ui.h \
    ../app/gl.h \
#    ../app/adaptor.h \
    ../app/example.h \
#    ../app/exampleadaptor.h \
    ../app/paramwidget.h \
    ../app/adaptorinterface.h \
    $$PWD/example2.h \
    $$PWD/functionaction.h \
    $$PWD/cube.h \
    $$PWD/geometry_interface.h \
    $$PWD/mesh.h \
    $$PWD/primatives.h

DISTFILES += \
    $$PWD/basicvertexshader.glsl \
    $$PWD/basicfragmentshader.glsl
