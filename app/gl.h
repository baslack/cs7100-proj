#ifndef GL_H
#define GL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_1>

class GL : public QOpenGLWidget, QOpenGLFunctions_3_1 {
    Q_OBJECT

  public:
    GL(QWidget *parent = Q_NULLPTR);
};

#endif // GL_H
