#include "cube.h"

cube::cube(GLfloat size) {
    GLfloat offset = size / 2.0f;
    m_points.push_back(QVector3D(-offset, -offset, -offset));
    m_points.push_back(QVector3D(-offset, -offset, offset));
    m_points.push_back(QVector3D(-offset, offset, offset));
    m_points.push_back(QVector3D(-offset, offset, -offset));
    m_points.push_back(QVector3D(offset, -offset, offset));
    m_points.push_back(QVector3D(offset, -offset, -offset));
    m_points.push_back(QVector3D(offset, offset, -offset));
    m_points.push_back(QVector3D(offset, offset, offset));

    m_faces.push_back(QVector<int> {1, 2, 3});
    m_faces.push_back(QVector<int> {1, 3, 4});
    m_faces.push_back(QVector<int> {2, 5, 8});
    m_faces.push_back(QVector<int> {2, 8, 3});
    m_faces.push_back(QVector<int> {5, 6, 7});
    m_faces.push_back(QVector<int> {5, 7, 8});
    m_faces.push_back(QVector<int> {6, 1, 4});
    m_faces.push_back(QVector<int> {6, 4, 7});
    m_faces.push_back(QVector<int> {4, 3, 8});
    m_faces.push_back(QVector<int> {4, 8, 7});
    m_faces.push_back(QVector<int> {6, 5, 2});
    m_faces.push_back(QVector<int> {6, 2, 1});

    for (auto face = m_faces.begin(); face != m_faces.end(); face++){
        QVector3D A = m_points[(*face)[0]-1] - m_points[(*face)[1]-1];
        QVector3D B = m_points[(*face)[2]-1] - m_points[(*face)[1]-1];
        QVector3D face_normal = QVector3D::crossProduct(A, B);
        face_normal.normalize();
        for (auto point = (*face).begin(); point != (*face).end(); point++){
            m_vertex_data.push_back(m_points[(*point)-1].x());
            m_vertex_data.push_back(m_points[(*point)-1].y());
            m_vertex_data.push_back(m_points[(*point)-1].z());
            m_normal_data.push_back(face_normal.x());
            m_normal_data.push_back(face_normal.y());
            m_normal_data.push_back(face_normal.z());
        }
    }
}

const GLfloat* cube::vertexData(void) const{
    return m_vertex_data.constData();
}

int cube::vertexDataCount(void) const{
    return m_vertex_data.size();
}

const GLfloat *cube::normalData() const
{
    return m_normal_data.constData();
}

int cube::normalDataCount() const
{
    return m_normal_data.size();
}
