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
    $$PWD/primatives.cpp \
    $$PWD/grid.cpp \
    $$PWD/derivatives.cpp \
    $$PWD/adaptivemesh.cpp

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
    $$PWD/primatives.h \
    $$PWD/grid.h \
    $$PWD/derivatives.h \
    $$PWD/adaptivemesh.h

DISTFILES += \
    $$PWD/basicvertexshader.glsl \
    $$PWD/basicfragmentshader.glsl \
    $$PWD/bluefrag.glsl \
    $$PWD/grid_fragshader.glsl \
    $$PWD/grid_vertshader.glsl \
    $$PWD/lambert_vertex.glsl \
    $$PWD/lambert_fragment.glsl
