#ifndef CUBE_H
#define CUBE_H

#include <qopengl.h>
#include <QVector>
#include <QVector3D>

class cube
{
public:
    cube(GLfloat);
    const GLfloat* constData(void);
    int count(void);
private:
    QVector<QVector<int>> faces;
    QVector<QVector3D> points;
    QVector<GLfloat> m_data;
};

#endif // CUBE_H
