#ifndef MESH_H
#define MESH_H

#include <QObject>
#include <QMatrix4x4>
#include <QException>

#include "geometry_interface.h"
#include "primatives.h"

class MeshAddPointFailed : public QException{
public:
    void raise(void) const {throw *this;}
    MeshAddPointFailed* clone(void) const{return new MeshAddPointFailed(*this);}
};

class Mesh : public QObject, public Geometry_Interface {
    Q_OBJECT
  public:
    explicit Mesh(int,
                  int,
                  QObject* parent = 0);
//    Mesh(void);
    ~Mesh();
    void addPoint(QVector3D);
    const Point& getPoint(int, int) const;
    void setPoint(int, int, Point&);
    QMatrix4x4 centeringTransform(void);
    QMatrix4x4 rangeScalingTransform(void);
    QVector3D boundsMin(void);
    QVector3D boundsMax(void);
    // implementations of interface
    void clear(void);
    const GLfloat* dataTrianglesPositions(void) const;
    const GLfloat* dataTrianglesNormals(void) const;
    const GLfloat* dataTrianglesUVWs(void) const;
    const GLfloat* dataPointPositions(void) const;
    int countTrianglesPositions(void) const;
    int countTrianglesNormals(void) const;
    int countTrianglesUVWs(void) const;
    int countPointPositions(void) const;
  private:
    // methods
    void pointbuffer(void);
    void triangulate(void);
    void updateBounds(QVector3D);
    // data
    int m_stepX;
    int m_stepY;
    int m_index_x;
    int m_index_y;
    QVector3D m_bounds_min;
    QVector3D m_bounds_max;
    QVector<Point> m_points;
    QVector<GLfloat> m_pt_pos;
    QVector<GLfloat> m_vertex_pos;
    QVector<GLfloat> m_vertex_n;
    QVector<GLfloat> m_vertex_uvw;
  signals:
    void MeshComplete(void);
    void MeshDataReady(void);
  public slots:

  private slots:
    void onMeshComplete(void);
};

#endif // MESH_H
