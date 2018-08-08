#ifndef MESH_H
#define MESH_H

#include <QObject>
#include <QMatrix4x4>

#include "geometry_interface.h"
#include "primatives.h"

enum retvals {
    SUCCESS,
    FAILURE
};

class Mesh : public QObject, Geometry_Interface {
    Q_OBJECT
  public:
    explicit Mesh(GLfloat,
                  GLfloat,
                  int,
                  GLfloat,
                  GLfloat,
                  int,
                  QObject* parent = 0);
    ~Mesh();
    int addPoint(QVector3D);
    const Point& getPoint(int, int) const;
    void setPoint(int, int, Point&);
    QMatrix4x4 centeringTransform(void);
    QMatrix4x4 rangeScalingTransform(void);
    QVector3D boundsMin(void);
    QVector3D boundsMax(void);
    // implementations of interface
    void clear(void);
    const GLfloat* dataTrianglesPositions(void) const;
    const GLfloat* datadataTrianglesNormals(void) const;
    const GLfloat* dataTrianglesUVWs(void) const;
    const GLfloat* dataPointPositions(void) const;
  private:
    // methods
    void pointbuffer(void);
    void triangulate(void);
    void updateBounds(QVector3D);
    // data
    GLfloat m_minX;
    GLfloat m_maxX;
    int m_stepX;
    GLfloat m_minY;
    GLfloat m_maxY;
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
