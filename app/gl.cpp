#include "gl.h"
#include "cube.h"
#include <QFile>

#define V_SHAD_PATH "../../proj/app/basicvertexshader.glsl"
#define F_SHAD_PATH "../../proj/app/basicfragmentshader.glsl"
#define NEAR_CLIP 0.1f
#define FAR_CLIP 100.0f

GL::GL(QWidget* parent):
    QOpenGLWidget (parent),
    m_view_target(QVector3D(0, 0, 0)),
    m_view_pos(QVector3D(10, 10, 10)),
    m_fov(28) {
    connect(
        this,
        &GL::ViewChanged,
        this,
        &GL::onViewChanged
    );
}

GL::~GL() {
    cleanup();
}

void GL::cleanup() {
    makeCurrent();
    m_vbo.destroy();
    delete m_prog;
    m_prog = 0;
    doneCurrent();
}

void GL::initializeGL(void) {
    // from the example, handles changing of widget parent recreation
    // likely not needed for this implementation
    connect(
        context(),
        &QOpenGLContext::aboutToBeDestroyed,
        this,
        &GL::cleanup
    );

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    // load and prepare the vertex shaders
    QFile v_shad_source_file(V_SHAD_PATH);
    v_shad_source_file.open(QIODevice::ReadOnly);
    QString v_shad_source = v_shad_source_file.readAll();
    v_shad_source_file.close();

    QFile f_shad_source_file(F_SHAD_PATH);
    f_shad_source_file.open(QIODevice::ReadOnly);
    QString f_shad_source = f_shad_source_file.readAll();
    f_shad_source_file.close();

    m_prog = new QOpenGLShaderProgram;
    m_prog->addShaderFromSourceCode(QOpenGLShader::Vertex, v_shad_source);
    m_prog->addShaderFromSourceCode(QOpenGLShader::Fragment, f_shad_source);
    m_prog->bindAttributeLocation("vertex", 0);
    m_prog->link();
    m_prog->bind();

    m_mvpMatLoc = m_prog->uniformLocation("mvpMat");

    // prepare the geo
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_vbo.create();
    m_vbo.bind();
    cube tempcube(5.0f);
    m_vbo.allocate(tempcube.constData(), tempcube.count() * sizeof(GLfloat));

    // setup Vertex Atrrs for shaders
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_vbo.release();

    // setup view mat
    m_viewMat.setToIdentity();
    m_viewMat.lookAt(m_view_pos, m_view_target, QVector3D(0, 1, 0));

    // release the vertex program?
    m_prog->release();
}

void GL::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_modelMat.setToIdentity();

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_prog->bind();
    QMatrix4x4 mvpMat = m_projectionMat * m_viewMat * m_modelMat;
    m_prog->setUniformValue(m_mvpMatLoc, mvpMat);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_prog->release();
}

void GL::resizeGL(int w, int h) {
    m_projectionMat.setToIdentity();
    m_projectionMat.perspective(m_fov, GLfloat(w) / h, NEAR_CLIP, FAR_CLIP);
}

void GL::mousePressEvent(QMouseEvent* event) {

}

void GL::mouseMoveEvent(QMouseEvent* event) {

}

// getters

QVector3D GL::ViewTarget() {
    return m_view_target;
}

QVector3D GL::ViewPos() {
    return m_view_pos;
}

GLfloat GL::FOV() {
    return m_fov;
}

// setters

void GL::setViewPos(QVector3D newpos) {
    m_view_pos = newpos;
    emit ViewChanged();
}

void GL::setViewTarget(QVector3D newtarg) {
    m_view_target = newtarg;
    emit ViewChanged();
}

void GL::setFOV(GLfloat newfov) {
    m_fov = newfov;
    emit ViewChanged();
}

// slots

void GL::onViewChanged() {
    m_viewMat.setToIdentity();
    m_viewMat.lookAt(m_view_pos, m_view_target, QVector3D(0, 1, 0));
    resizeGL(this->geometry().width(), this->geometry().height());
}
