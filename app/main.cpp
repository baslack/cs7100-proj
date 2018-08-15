#include <QApplication>
#include <QSurfaceFormat>
#include <iostream>
#include "ui.h"

void setupOpenGL(int, int, int);

int main(int argc, char** argv) {
    setupOpenGL(3, 1, 4);
    QApplication app(argc, argv);
    ui ui;
    ui.setup();
    ui.show();
    int test = app.exec();
//    return app.exec();
    return test;
}

void setupOpenGL(int major, int minor, int samples) {
    QSurfaceFormat format;

    format.setMajorVersion(major);
    format.setMinorVersion(minor);
    format.setSamples(samples);

    format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
//    format.setOption(QSurfaceFormat::FormatOption::DebugContext);
    format.setSwapBehavior(QSurfaceFormat::SwapBehavior::DefaultSwapBehavior);
    format.setSwapInterval(1);
    format.setRedBufferSize(8);
    format.setBlueBufferSize(8);
    format.setGreenBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setStencilBufferSize(8);

    QSurfaceFormat::setDefaultFormat(format);
}
