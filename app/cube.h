#ifndef CUBE_H
#define CUBE_H

#include <qopengl.h>
#include <QVector>
#include <QVector3D>

class cube
{
public:
    cube(GLfloat);
    const GLfloat* vertexData(void) const;
    int vertexDataCount(void) const;
    const GLfloat* normalData(void) const;
    int normalDataCount(void) const;
private:
    QVector<QVector<int>> m_faces;
    QVector<QVector3D> m_points;
    QVector<GLfloat> m_vertex_data;
    QVector<GLfloat> m_normal_data;
};

#endif // CUBE_H
