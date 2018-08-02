#include "cube.h"

cube::cube(GLfloat size) {
    GLfloat offset = size / 2.0f;
    points.push_back(QVector3D(-offset, -offset, -offset));
    points.push_back(QVector3D(-offset, -offset, offset));
    points.push_back(QVector3D(-offset, offset, offset));
    points.push_back(QVector3D(-offset, offset, -offset));
    points.push_back(QVector3D(offset, -offset, offset));
    points.push_back(QVector3D(offset, -offset, -offset));
    points.push_back(QVector3D(offset, offset, -offset));
    points.push_back(QVector3D(offset, offset, offset));

    faces.push_back(QVector<int> {1, 2, 3});
    faces.push_back(QVector<int> {1, 3, 4});
    faces.push_back(QVector<int> {2, 5, 8});
    faces.push_back(QVector<int> {2, 8, 3});
    faces.push_back(QVector<int> {5, 6, 7});
    faces.push_back(QVector<int> {5, 7, 8});
    faces.push_back(QVector<int> {6, 1, 4});
    faces.push_back(QVector<int> {6, 4, 7});
    faces.push_back(QVector<int> {4, 3, 8});
    faces.push_back(QVector<int> {4, 8, 7});
    faces.push_back(QVector<int> {6, 5, 2});
    faces.push_back(QVector<int> {6, 2, 1});

    for (auto face = faces.begin(); face != faces.end(); face++){
        for (auto point = (*face).begin(); point != (*face).end(); point++){
            m_data.push_back(points[(*point)-1].x());
            m_data.push_back(points[(*point)-1].y());
            m_data.push_back(points[(*point)-1].z());
        }
    }
}

const GLfloat* cube::constData(void){
    return m_data.constData();
}

int cube::count(void){
    return m_data.size();
}
