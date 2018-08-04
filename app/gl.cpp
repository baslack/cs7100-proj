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
    m_view_up(QVector3D(0, 1, 0)),
    m_fov(28),
    m_last_pos(QPointF(0, 0)) {
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
    m_viewMat.lookAt(m_view_pos, m_view_target, m_view_up);

    // init model mat
    m_modelMat.setToIdentity();


    // release the vertex program?
    m_prog->release();
}

void GL::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //    m_modelMat.setToIdentity();

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

QVector3D GL::calculateOffset(QPointF current_pos) {
    qreal width(this->geometry().width());
    qreal height(this->geometry().height());
    // convert the points to camera space coords
    float curpos_x((current_pos.x() - width / 2) / (width / 2));
    float curpos_y((current_pos.y() - height / 2) / (height / 2));
    float lastpos_x = ((m_last_pos.x() - width / 2) / (width / 2));
    float lastpos_y = ((m_last_pos.y() - height / 2) / (height / 2));
    // calculate the offset vector
    float offset_x = curpos_x - lastpos_x;
    float offset_y = -(curpos_y - lastpos_y);
    return QVector3D(offset_x, offset_y, 0);
}

QVector3D GL::arcballVector(QPointF pos) {
    float radius = 1.0f;
    float width = this->geometry().width();
    float x = -((width - pos.x()) / width * 2 - 1);
    float height = this->geometry().height();
    float y = ((height - pos.y()) / height * 2 - 1);
    float sqXY = x * x + y * y;
    float sqR = radius * radius;
    float z = 0.0f;
    if (sqXY <= sqR / 2) {
        z = qSqrt(sqR - sqXY);
    } else {
        z = (sqR / 2) / qSqrt(sqXY);
    }
    return QVector3D(x, y, z).normalized();
}

void GL::mousePressEvent(QMouseEvent* event) {
        m_last_pos = event->localPos();
}

void GL::mouseMoveEvent(QMouseEvent* event) {
    QVector3D offset = calculateOffset(event->localPos());

    switch (event->modifiers()) {
        case Qt::ControlModifier:
            switch (event->buttons()) {
                case Qt::LeftButton:
                    performTumble(event->localPos());
                    break;
                case Qt::MiddleButton:
                    performTrack(offset);
                    break;
                case Qt::RightButton:
                    performTruck(offset);
                    break;
                default:
                    break;
            }
            break;
        case Qt::ControlModifier|Qt::ShiftModifier:
            switch (event->buttons()) {
                case Qt::LeftButton:
                    performRoll(offset);
                    break;
                case Qt::MiddleButton:
                    performTrack(offset);
                    break;
                case Qt::RightButton:
                    performZoom(offset);
                    break;
                default:
                    break;
            }
            break;
        //        case Qt::AltModifier|Qt::ControlModifier:
        //            switch (event->buttons()) {
        //                case Qt::LeftButton:
        //                    performTumbleAboutCenter(offset);
        //                    break;
        //                case Qt::MiddleButton:
        //                    performTrack(offset);
        //                    break;
        //                case Qt::RightButton:
        //                    performTruck(offset);
        //                    break;
        //                default:
        //                    break;
        //            }
        //            break;
        default:
            break;
    }
    m_last_pos = event->localPos();
    update();
    //    emit ViewChanged();
}

void GL::performTumble(QPointF current_pos) {
    // rotate the view about the center
    // of the cam
    QVector3D va = arcballVector(m_last_pos);
    QVector3D vb = arcballVector(current_pos);
    QVector3D axis_cs = QVector3D::crossProduct(va, vb);
//    qDebug() << axis_cs;
    axis_cs.normalize();
    QVector4D _axis_cs(axis_cs, 0.0f);
    float angle  = qAcos(qMin(1.0f, QVector3D::dotProduct(va, vb)));
    float degrees(qRadiansToDegrees(angle));
    QMatrix4x4 MV(m_viewMat * m_modelMat);
    QMatrix4x4 invMV = MV.inverted();
    QVector4D _axis_os = invMV * _axis_cs;
    QVector3D axis_os = _axis_os.toVector3D();
//    qDebug() << axis_os;
    axis_os.normalize();
    m_modelMat.rotate(degrees, axis_os);
    //    m_viewMat.rotate(degrees, axis_cs);
}

void GL::performTumbleAboutCenter(QVector3D offset) {

}

void GL::performTrack(QVector3D offset) {
    // translate the view in camera space xy
    // then move the translation back into ws
}

void GL::performTruck(QVector3D offset) {
    // translate the view in camera space z
    //then move the translation back into ws
}

void GL::performZoom(QVector3D offset) {
    // change the fov
}

void GL::performRoll(QVector3D offset) {
    // rotate the up vector about the z camera space
    // then

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
    m_viewMat.lookAt(m_view_pos, m_view_target, m_view_up);
    resizeGL(this->geometry().width(), this->geometry().height());
}
