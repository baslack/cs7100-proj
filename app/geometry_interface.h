#ifndef GEOMETRY_INTERFACE_H
#define GEOMETRY_INTERFACE_H

#include <qopengl.h>

class Geometry_Interface {
  public:
    virtual ~Geometry_Interface() {};
    virtual void clear(void) = 0;
    virtual const GLfloat* dataTrianglesPositions(void) const = 0;
    virtual int countTrianglesPositions(void) const = 0;
    virtual const GLfloat* dataTrianglesNormals(void) const = 0;
    virtual int countTrianglesNormals(void) const = 0;
    virtual const GLfloat* dataTrianglesUVWs(void) const = 0;
    virtual int countTrianglesUVWs(void) const = 0;
    virtual const GLfloat* dataPointPositions(void) const = 0;
    virtual int countPointPositions(void) const = 0;
};

#endif // GEOMETRY_INTERFACE_H
