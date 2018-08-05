#ifndef GL_H
#define GL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_1>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QPointF>
#include <QMouseEvent>
//#include <cmath>
#include <QtMath>

class GL : public QOpenGLWidget, protected QOpenGLFunctions_3_1 {
    Q_OBJECT
    Q_PROPERTY(QVector3D ViewTarget READ ViewTarget WRITE setViewTarget)
    Q_PROPERTY(QVector3D ViewPos READ ViewPos WRITE setViewPos)
    Q_PROPERTY(float FOV READ FOV WRITE setFOV)
  signals:
    void ViewChanged(void);
  public slots:
    void cleanup(void);

  private slots:
    void onViewChanged(void);

  public:
    GL(QWidget* parent = Q_NULLPTR);
    ~GL();
    //getters
    QVector3D ViewTarget(void);
    QVector3D ViewPos(void);
    float FOV(void);
    // setters
    void setViewTarget(QVector3D);
    void setViewTarget(float, float, float);
    void setViewPos(QVector3D);
    void setViewPos(float, float, float);
    void setFOV(float);

  private:
    // view data members
    QVector3D m_view_target;  // ws locatoin of center of view
    QVector3D m_view_pos; // ws location of viewing position
    QVector3D m_view_up;  // ws up vector of the view
    float m_fov; // field of view
    QMatrix4x4 m_projectionMat;  // perspective projection
    QMatrix4x4 m_viewMat;  // worlds space to camera space
    QMatrix4x4 m_modelMat;  // model space to worlds space, i.e. transform
    // mouse data members
    QPointF m_last_pos;
    // visualized points data members
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    // shader data members
    QOpenGLShaderProgram* m_prog;
    // shader memory locations
    int m_mvpMatLoc;
    // ui event data members

  protected:
    // virtuals from OpenGLWidget to implement
    void initializeGL(void);
    void paintGL(void);
    void resizeGL(int, int);
    // virtuals for QWidget to track the mouse movement
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void performArcballTumble(QPointF);
    void performTumble(QPointF);
    void performTrack(QPointF);
    void performTruck(QPointF);
    void performZoom(QPointF);
    void performRoll(QPointF);
    QVector3D arcballVector(QPointF);
};

#endif // GL_H
