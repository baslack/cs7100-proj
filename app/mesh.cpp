#include "mesh.h"

Mesh::Mesh(GLfloat minX,
           GLfloat maxX,
           int stepX,
           GLfloat minY,
           GLfloat maxY,
           int stepY,
           QObject* parent):
    QObject(parent),
    m_minX(minX),
    m_maxX(maxX),
    m_stepX(stepX),
    m_minY(minY),
    m_maxY(maxY),
    m_stepY(stepY),
    m_index_x(0),
    m_index_y(0),
    m_bounds_min(0, 0, 0),
    m_bounds_max(0, 0, 0) {
    int num_points = m_stepX * m_stepY;
    m_points.reserve(num_points);
    for (int i = 0; i < num_points; i++) {
        m_points.push_back(Point(QVector3D(0, 0, 0)));
    }
    connect(
        this,
        &Mesh::MeshComplete,
        this,
        &Mesh::onMeshComplete
    );
}

Mesh::~Mesh() {

}

int Mesh::addPoint(
    QVector3D ws_position) {
    if (m_index_x >= m_stepX) {
        return FAILURE;
    }
    Point new_point(ws_position);
    setPoint(m_index_x, m_index_y, new_point);
    m_index_y++;
    if (m_index_y == m_stepY) {
        m_index_y = 0;
        m_index_x++;
    }
    if (m_index_x == m_stepX) {
        emit MeshComplete();
    }
    return SUCCESS;
}

const Point& Mesh::getPoint(
    int coord_x,
    int coord_y
)
const {
    return m_points[m_stepY * coord_x + coord_y];
}

void Mesh::setPoint(
    int coord_x,
    int coord_y,
    Point& new_pt) {
    m_points[m_stepY * coord_x + coord_y] = new_pt;
    updateBounds(new_pt.getPosition());
}

QMatrix4x4 Mesh::centeringTransform() {
    GLfloat midpoint_x = (m_maxX - m_minX) / 2 + m_minX;
    GLfloat midpoint_y = (m_maxY - m_minY) / 2 + m_minY;
    QMatrix4x4 retMat;
    retMat.setToIdentity();
    retMat.translate(QVector3D(-midpoint_x, -midpoint_y, 0));
    return retMat;
}

QMatrix4x4 Mesh::rangeScalingTransform() {
    QVector3D mins = boundsMin();
    QVector3D maxs = boundsMax();
    GLfloat range_x = maxs.x() - mins.x();
    GLfloat range_y = maxs.y() - mins.y();
    GLfloat range_z = maxs.z() - mins.z();
    GLfloat scale_z = 1.0f;
    if (range_z / range_x > range_z / range_y) {
        scale_z = range_x / range_z;
    } else {
        scale_z = range_y / range_z;
    }
    QMatrix4x4 retMat;
    retMat.setToIdentity();
    retMat.scale(QVector3D(1.0f, 1.0f, scale_z));
    return retMat;
}

void Mesh::updateBounds(QVector3D pos) {
    if (pos.x() < m_bounds_min.x()) {
        m_bounds_min.setX(pos.x());
    }
    if (pos.x() > m_bounds_max.x()) {
        m_bounds_max.setX(pos.x());
    }
    if (pos.y() < m_bounds_min.y()) {
        m_bounds_min.setY(pos.y());
    }
    if (pos.y() > m_bounds_max.y()) {
        m_bounds_max.setY(pos.y());
    }
    if (pos.z() < m_bounds_min.z()) {
        m_bounds_min.setZ(pos.z());
    }
    if (pos.z() > m_bounds_max.z()) {
        m_bounds_max.setZ(pos.z());
    }
}

void Mesh::pointbuffer() {
    for (int i = 0;
            i < m_stepX;
            i++) {
        for (int j = 0;
                j < m_stepY;
                j++) {
            const Point& pt = getPoint(i, j);
            m_pt_pos.push_back(pt.getPosition().x());
            m_pt_pos.push_back(pt.getPosition().y());
            m_pt_pos.push_back(pt.getPosition().z());
        }
    }
}

void Mesh::triangulate() {
    for (int i = 0; i < (m_stepX - 1); i++) {
        for (int j = 0; j < (m_stepY - 1); j++) {
            // get the points that will make up the quad face
            const Point& A = getPoint(i, j);
            const Point& B = getPoint(i + 1, j);
            const Point& C = getPoint(i + 1, j + 1);
            const Point& D = getPoint(i, j + 1);
            // pack the vertex positions
            m_vertex_pos.push_back(A.getPosition().x());
            m_vertex_pos.push_back(A.getPosition().y());
            m_vertex_pos.push_back(A.getPosition().z());
            m_vertex_pos.push_back(B.getPosition().x());
            m_vertex_pos.push_back(B.getPosition().y());
            m_vertex_pos.push_back(B.getPosition().z());
            m_vertex_pos.push_back(C.getPosition().x());
            m_vertex_pos.push_back(C.getPosition().y());
            m_vertex_pos.push_back(C.getPosition().z());
            m_vertex_pos.push_back(A.getPosition().x());
            m_vertex_pos.push_back(A.getPosition().y());
            m_vertex_pos.push_back(A.getPosition().z());
            m_vertex_pos.push_back(C.getPosition().x());
            m_vertex_pos.push_back(C.getPosition().y());
            m_vertex_pos.push_back(C.getPosition().z());
            m_vertex_pos.push_back(D.getPosition().x());
            m_vertex_pos.push_back(D.getPosition().y());
            m_vertex_pos.push_back(D.getPosition().z());
            // work out normals
            QVector3D AB = B.getPosition() - A.getPosition();
            QVector3D BA = -AB;
            QVector3D BC = C.getPosition() - B.getPosition();
            QVector3D CB = -BC;
            QVector3D CA = A.getPosition() - C.getPosition();
            QVector3D AC = -CA;
            QVector3D nBAC = QVector3D::crossProduct(AB, AC).normalized();
            QVector3D nABC = QVector3D::crossProduct(BC, BA).normalized();
            QVector3D nBCA = QVector3D::crossProduct(CA, CB).normalized();
            QVector3D CD = D.getPosition() - C.getPosition();
            QVector3D DC = -CD;
            QVector3D DA = A.getPosition() - D.getPosition();
            QVector3D AD = -DA;
            QVector3D nCAD = QVector3D::crossProduct(AC, AD).normalized();
            QVector3D nDCA = QVector3D::crossProduct(CD, CA).normalized();
            QVector3D nADC = QVector3D::crossProduct(DA, DC).normalized();
            // pack the normals ABC
            m_vertex_n.push_back(nBAC.x());
            m_vertex_n.push_back(nBAC.y());
            m_vertex_n.push_back(nBAC.z());
            m_vertex_n.push_back(nABC.x());
            m_vertex_n.push_back(nABC.y());
            m_vertex_n.push_back(nABC.z());
            m_vertex_n.push_back(nBCA.x());
            m_vertex_n.push_back(nBCA.y());
            m_vertex_n.push_back(nBCA.z());
            // pack the normals ACD
            m_vertex_n.push_back(nCAD.x());
            m_vertex_n.push_back(nCAD.y());
            m_vertex_n.push_back(nCAD.z());
            m_vertex_n.push_back(nDCA.x());
            m_vertex_n.push_back(nDCA.y());
            m_vertex_n.push_back(nDCA.z());
            m_vertex_n.push_back(nADC.x());
            m_vertex_n.push_back(nADC.y());
            m_vertex_n.push_back(nADC.z());
            // generate the uvws
            GLfloat uv_stepV = 1.0f / m_stepX;
            GLfloat uv_stepU = 1.0f / m_stepY;
            QVector3D uvBAC(uv_stepU * j, 1.0f - uv_stepV * i, 0);
            QVector3D uvABC(uv_stepU * j, 1.0f - uv_stepV * (i + 1), 0);
            QVector3D uvBCA(uv_stepU * (j + 1), 1.0f - uv_stepV * (i + 1), 0);
            QVector3D uvCAD = uvBAC;
            QVector3D uvDCA = uvBCA;
            QVector3D uvADC(uv_stepU * (j + 1), 1.0f - uv_stepV * i, 0);
            // pack the uvs ABC
            m_vertex_uvw.push_back(uvBAC.x());
            m_vertex_uvw.push_back(uvBAC.y());
            m_vertex_uvw.push_back(uvBAC.z());
            m_vertex_uvw.push_back(uvABC.x());
            m_vertex_uvw.push_back(uvABC.y());
            m_vertex_uvw.push_back(uvABC.z());
            m_vertex_uvw.push_back(uvBCA.x());
            m_vertex_uvw.push_back(uvBCA.y());
            m_vertex_uvw.push_back(uvBCA.z());
            // pack the uvw ACD
            m_vertex_uvw.push_back(uvCAD.x());
            m_vertex_uvw.push_back(uvCAD.y());
            m_vertex_uvw.push_back(uvCAD.z());
            m_vertex_uvw.push_back(uvDCA.x());
            m_vertex_uvw.push_back(uvDCA.y());
            m_vertex_uvw.push_back(uvDCA.z());
            m_vertex_uvw.push_back(uvADC.x());
            m_vertex_uvw.push_back(uvADC.y());
            m_vertex_uvw.push_back(uvADC.z());
        }
    }
}

void Mesh::onMeshComplete() {
    pointbuffer();
    triangulate();
    emit MeshDataReady();
}

void Mesh::clear() {
    m_index_x = 0;
    m_index_y = 0;
    m_points.clear();
    m_points.reserve(m_stepX * m_stepY);
    for(int i = 0; i < m_stepX; i++) {
        for (int j = 0; j < m_stepY; j++) {
            Point new_pt(QVector3D(0, 0, 0));
            setPoint(i, j, new_pt);
        }
    }
    m_bounds_max = QVector3D(0, 0, 0);
    m_bounds_min = QVector3D(0, 0, 0);
    m_pt_pos.clear();
    m_vertex_pos.clear();
    m_vertex_n.clear();
    m_vertex_uvw.clear();
}

const GLfloat* Mesh::dataPointPositions(void) const {
    return m_pt_pos.constData();
}

const GLfloat* Mesh::dataTrianglesPositions(void) const {
    return m_vertex_pos.constData();
}

const GLfloat* Mesh::datadataTrianglesNormals(void) const {
    return m_vertex_n.constData();
}

const GLfloat* Mesh::dataTrianglesUVWs(void) const{
    return m_vertex_uvw.constData();
}
