#include "gl.h"
#include "cube.h"
#include "grid.h"
#include <QFile>

#define V_SHAD_PATH "../../proj/app/basicvertexshader.glsl"
#define F_SHAD_PATH "../../proj/app/basicfragmentshader.glsl"
#define F_SHAD_BLUE_PATH "../../proj/app/bluefrag.glsl"
#define V_SHAD_GRID_PATH "../../proj/app/grid_vertshader.glsl"
#define F_SHAD_GRID_PATH "../../proj/app/grid_fragshader.glsl"
#define NEAR_CLIP 0.1f
#define FAR_CLIP 100.0f

GL::GL(QWidget* parent):
    QOpenGLWidget (parent),
    m_view_target(QVector3D(0, 0, 0)),
    m_view_pos(QVector3D(10, 10, 10)),
    m_view_up(QVector3D(0, 1, 0)),
    m_fov(28),
    m_last_pos(QPointF(0, 0)) {
    //    connect(
    //        this,
    //        &GL::ViewChanged,
    //        this,
    //        &GL::onViewChanged
    //    );
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
    m_prog->bindAttributeLocation("normal", 1);
    m_prog->bindAttributeLocation("uvw", 2);
    m_prog->bindAttributeLocation("point", 3);
    m_prog->link();
    m_prog->bind();
    m_mvpMatLoc = m_prog->uniformLocation("mvpMat");
    m_prog->release();


    QFile f_blue_source_file(F_SHAD_BLUE_PATH);
    f_blue_source_file.open(QIODevice::ReadOnly);
    QString f_blue_source = f_blue_source_file.readAll();
    f_blue_source_file.close();

    m_prog2 = new QOpenGLShaderProgram;
    m_prog2->addShaderFromSourceCode(QOpenGLShader::Vertex, v_shad_source);
    m_prog2->addShaderFromSourceCode(QOpenGLShader::Fragment, f_blue_source);
    m_prog2->bindAttributeLocation("vertex", 0);
    m_prog2->link();
    m_prog2->bind();
    m_mvpMatLoc2 = m_prog2->uniformLocation("mvpMat");
    m_prog2->release();

    // prep the base grid
    QFile v_grid_shad_source_file(V_SHAD_GRID_PATH);
    v_grid_shad_source_file.open(QIODevice::ReadOnly);
    QString v_grid_shad_source = v_grid_shad_source_file.readAll();
    v_grid_shad_source_file.close();
    QFile f_grid_shad_source_file(F_SHAD_GRID_PATH);
    f_grid_shad_source_file.open(QIODevice::ReadOnly);
    QString f_grid_shad_source = f_grid_shad_source_file.readAll();
    f_grid_shad_source_file.close();

    m_prog_grid = new QOpenGLShaderProgram;
    m_prog_grid->addShaderFromSourceCode(QOpenGLShader::Vertex, v_grid_shad_source);
    m_prog_grid->addShaderFromSourceCode(QOpenGLShader::Fragment, f_grid_shad_source);
    m_prog_grid->bindAttributeLocation("vp", 0);
    m_prog_grid->bindAttributeLocation("vc", 1);
    m_prog_grid->link();
    m_prog_grid->bind();
    m_mvpMatLoc_grid = m_prog_grid->uniformLocation("mvpMat");
    m_prog_grid->release();

    m_vao_grid.create();
    m_vao_grid.bind();
    grid _grid(5.0f, 10);
    m_vbo_grid.create();
    m_vbo_grid.bind();
    m_vbo_grid.allocate(_grid.getLineData(), _grid.getLineCount()*sizeof(GLfloat));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    m_vbo_grid.release();
    m_vao_grid.release();

    // prepare the geo
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    //    m_vbo.create();
    //    m_vbo.bind();

    // fill the vertex buffer with a default cube
    cube tempcube(5.0f);

    // create the buffers
    m_vertex_buffer.create();
    m_vertex_buffer.bind();
    m_vertex_buffer.allocate(tempcube.constData(), tempcube.count() * sizeof(GLfloat));
    qDebug() << "vb init size: " << m_vertex_buffer.size();
    qDebug() << "usage pattern" << QString::number(m_vertex_buffer.usagePattern(), 16);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_vertex_buffer.release();

    m_normal_buffer.create();
    m_normal_buffer.bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_normal_buffer.release();

    m_uvw_buffer.create();
    m_uvw_buffer.bind();
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_uvw_buffer.release();

    m_pt_buffer.create();
    m_pt_buffer.bind();
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_pt_buffer.release();


    //    m_vbo.allocate(tempcube.constData(), tempcube.count() * sizeof(GLfloat));

    //    m_vbo.release();

    // setup view mat
    m_viewMat.setToIdentity();
    m_viewMat.lookAt(m_view_pos, m_view_target, m_view_up);

    // init mats
    m_modelMat.setToIdentity();
    m_centeringMat.setToIdentity();
    m_rangescaleMat.setToIdentity();


    // release the vertex program?
    //    m_prog->release();
}

void GL::paintGL() {
    QMatrix4x4 mvpMat;
    QMatrix4x4 y_on_horz;
    y_on_horz.setToIdentity();
    y_on_horz.rotate(-90.0f, QVector3D(1, 0, 0));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4.0f);
    //    glEnable(GL_LINE_SMOOTH);
    //    glLineWidth(2.0f);

    //    m_modelMat.setToIdentity();

    m_vao_grid.bind();
    m_prog_grid->bind();
    mvpMat = m_projectionMat * m_viewMat * m_modelMat * y_on_horz;
    m_prog_grid->setUniformValue(m_mvpMatLoc_grid, mvpMat);
    m_vbo_grid.bind();
    glDrawArrays(GL_LINES, 0, m_vbo_grid.size() / int(sizeof(GLfloat)) / 3 / 2);
    m_vbo_grid.release();
    m_prog_grid->release();
    m_vao_grid.release();


    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_prog->bind();
    //    QMatrix4x4 mvpMat = m_projectionMat * m_viewMat * m_modelMat;

    mvpMat = m_projectionMat
             * m_viewMat
             * m_modelMat
             * y_on_horz
             * m_rangescaleMat
             * m_centeringMat;

    m_prog->setUniformValue(m_mvpMatLoc, mvpMat);

    m_vertex_buffer.bind();
    //    qDebug() << "bound vb: " << QString::number(m_vertex_buffer.bind());
    //    qDebug() << "vb size:" << m_vertex_buffer.size();
    //    qDebug() << "size of glfloat << " << sizeof(GLfloat);
    if (m_vertex_buffer.size() != 0) {
        glDrawArrays(GL_TRIANGLES, 0, m_vertex_buffer.size() / int(sizeof(GLfloat)) / 3);
    }
    m_prog->release();
    m_prog2->bind();
    m_prog2->setUniformValue(m_mvpMatLoc2, mvpMat);

    if (m_vertex_buffer.size() != 0) {
        glDrawArrays(GL_POINTS, 0, m_vertex_buffer.size() / int(sizeof(GLfloat)) / 3);
    }
    m_vertex_buffer.release();
    m_prog2->release();
}

void GL::resizeGL(int w, int h) {
    m_projectionMat.setToIdentity();
    m_projectionMat.perspective(m_fov, GLfloat(w) / h, NEAR_CLIP, FAR_CLIP);
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
    switch (event->modifiers()) {
        case Qt::ControlModifier:
            switch (event->buttons()) {
                case Qt::LeftButton:
                    performTumble(event->localPos());
                    break;
                case Qt::MiddleButton:
                    performTrack(event->localPos());
                    break;
                case Qt::RightButton:
                    performTruck(event->localPos());
                    break;
                default:
                    break;
            }
            break;
        case Qt::ControlModifier|Qt::ShiftModifier:
            switch (event->buttons()) {
                case Qt::LeftButton:
                    performRoll(event->localPos());
                    break;
                case Qt::MiddleButton:
                    performTrack(event->localPos());
                    break;
                case Qt::RightButton:
                    performZoom(event->localPos());
                    break;
                default:
                    break;
            }
            break;
        case Qt::ShiftModifier:
            switch (event->buttons()) {
                case Qt::LeftButton:
                    performArcballTumble(event->localPos());
                    break;
                case Qt::MiddleButton:
                    performTrack(event->localPos());
                    break;
                case Qt::RightButton:
                    performTruck(event->localPos());
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    m_last_pos = event->localPos();
    update();
    //    emit ViewChanged();
}

void GL::performArcballTumble(QPointF current_pos) {
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

void GL::performTumble(QPointF current_pos) {
    // get screen space vectors from last and current pos
    QVector3D va = arcballVector(m_last_pos);
    QVector3D vb = arcballVector(current_pos);
    //    qDebug() << "va:" << va << " vb:" << vb;
    QVector3D rot_axis = QVector3D::crossProduct(va, vb);
    float angle = qAcos(qMin(1.0f, QVector3D::dotProduct(va, vb)));
    QMatrix4x4 rot_mat;
    float rot_scalar = 0.4f;
    rot_mat.setToIdentity();
    rot_mat.rotate(rot_scalar * qRadiansToDegrees(angle), rot_axis);
    m_viewMat = rot_mat * m_viewMat;
}

void GL::performTrack(QPointF current_pos) {
    // translate the view in camera space xy
    // then move the translation back into ws
    // get screen space vectors from last and current pos
    QVector3D va = arcballVector(m_last_pos);
    QVector3D vb = arcballVector(current_pos);
    float x = vb.x() - va.x();
    float y = vb.y() - va.y();
    float translation_scalar = 5.0f;
    QMatrix4x4 trans_mat;
    trans_mat.setToIdentity();
    trans_mat.translate(QVector3D(x, y, 0)*translation_scalar);
    m_viewMat = trans_mat * m_viewMat;
}

void GL::performTruck(QPointF current_pos) {
    // translate the view in camera space z
    //then move the translation back into ws
    QVector3D va = arcballVector(m_last_pos);
    QVector3D vb = arcballVector(current_pos);
    float z = vb.y() - va.y();
    float translation_scalar = 5.0f;
    QMatrix4x4 trans_mat;
    trans_mat.setToIdentity();
    trans_mat.translate(QVector3D(0, 0, z)*translation_scalar);
    m_viewMat = trans_mat * m_viewMat;
}

void GL::performZoom(QPointF current_pos) {
    // change the fov
    QVector3D va = arcballVector(m_last_pos);
    QVector3D vb = arcballVector(current_pos);
    float adj_fov = vb.y() - va.y();
    float fov_scalar = 10.0f;
    m_fov += fov_scalar * adj_fov;
    resizeGL(this->geometry().width(), this->geometry().height());
}

void GL::performRoll(QPointF current_pos) {
    // rotate the up vector about the z camera space
    // get screen space vectors from last and current pos
    QVector3D va = arcballVector(m_last_pos);
    QVector3D vb = arcballVector(current_pos);
    //    qDebug() << "va:" << va << " vb:" << vb;
    QVector3D rot_axis(0, 0, 1);
    float angle = qAcos(qMin(1.0f, QVector3D::dotProduct(va, vb)));
    QMatrix4x4 rot_mat;
    float rot_scalar = 0.4f;
    if (vb.x() > va.x()) {
        rot_scalar *= -1;
    }
    rot_mat.setToIdentity();
    rot_mat.rotate(rot_scalar * qRadiansToDegrees(angle), rot_axis);
    m_viewMat = rot_mat * m_viewMat;
}


void GL::updateVertexBuffer(const GLfloat* data, int count) {
    makeCurrent();
    m_vertex_buffer.bind();
    m_vertex_buffer.allocate(data, count * sizeof(GLfloat));
    m_vertex_buffer.release();
}

void GL::updateNormalBuffer(const GLfloat* data, int count) {
    makeCurrent();
    m_normal_buffer.bind();
    m_normal_buffer.allocate(data, count * sizeof(GLfloat));
    m_normal_buffer.release();
}

void GL::updateUVWBuffer(const GLfloat* data, int count) {
    makeCurrent();
    m_uvw_buffer.bind();
    m_uvw_buffer.allocate(data, count * sizeof(GLfloat));
    m_uvw_buffer.release();
}

void GL::updatePointBuffer(const GLfloat* data, int count) {
    makeCurrent();
    m_pt_buffer.bind();
    m_pt_buffer.allocate(data, count * sizeof(GLfloat));
    m_pt_buffer.release();
}

void GL::forceUpdate() {
    makeCurrent();
    update();
}

void GL::setCentering(GLfloat centering) {
    makeCurrent();
    m_centering = centering;
    update();
}

void GL::setRangeScaling(GLfloat range_scaling) {
    makeCurrent();
    m_range_scaling = range_scaling;
    update();
}

QMatrix4x4 GL::CenteringMat(void) {
    QMatrix4x4 retVal, Identity;
    Identity.setToIdentity();
    retVal = (1 - m_centering) * Identity + (m_centering) * m_centeringMat;
    return retVal;
}

QMatrix4x4 GL::RangeScaleMat(void) {
    QMatrix4x4 retVal, Identity;
    Identity.setToIdentity();
    retVal = (1 - m_range_scaling) * Identity + (m_range_scaling) * m_rangescaleMat;
    return retVal;
}

void GL::setPointsOnly(bool state){
    m_pts_only = state;
}

// getters

//QVector3D GL::ViewTarget() {
//    return m_view_target;
//}

//QVector3D GL::ViewPos() {
//    return m_view_pos;
//}

//GLfloat GL::FOV() {
//    return m_fov;
//}

// setters

//void GL::setViewPos(QVector3D newpos) {
//    m_view_pos = newpos;
//    emit ViewChanged();
//}

//void GL::setViewTarget(QVector3D newtarg) {
//    m_view_target = newtarg;
//    emit ViewChanged();
//}

//void GL::setFOV(GLfloat newfov) {
//    m_fov = newfov;
//    emit ViewChanged();
//}

// slots

//void GL::onViewChanged() {
//    m_viewMat.setToIdentity();
//    m_viewMat.lookAt(m_view_pos, m_view_target, m_view_up);
//    resizeGL(this->geometry().width(), this->geometry().height());
//}
