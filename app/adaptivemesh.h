#ifndef ADAPTIVEMESH_H
#define ADAPTIVEMESH_H

#include <QObject>
#include "geometry_interface.h"
#include "adaptorinterface.h"
#include <qopengl.h>
#include "derivatives.h"
#include <QVector>
#include <QVector2D>

class Region {
  public:
    Region(QVector2D, QVector2D);
    QVector2D getMidpoint(void) const;
    QVector2D getMinpoint(void) const;
    QVector2D getMaxpoint(void) const;
    void setMinpoint(QVector2D);
    void setMaxpoint(QVector2D);
    double getMaxDerivative(void) const;
    bool getMaxOnX(void) const;
    void setMaxDerivative(bool, double);
    bool operator< (const Region&) const;
    static bool Compare(const Region&, const Region&);
    double getArea(void) const;
    QVector<GLfloat> getLines(void) const;
  private:
    QVector2D m_min;
    QVector2D m_max;
    bool m_max_on_X;
    double m_max_derivative;
};

class AdaptiveMesh : public QObject, public Geometry_Interface {
    Q_OBJECT
  public:
    explicit AdaptiveMesh(
        AdaptorInterface*,
        const QVector<double>&,
        int,
        int,
        QVector2D,
        QVector2D,
        int,
        GLfloat,
        GLfloat,
        QObject* parent = 0
    );
    void clear(void);
    const GLfloat* dataTrianglesPositions(void) const;
    int countTrianglesPositions(void) const;
    const GLfloat* dataTrianglesNormals(void) const;
    int countTrianglesNormals(void) const;
    const GLfloat* dataTrianglesUVWs(void) const;
    int countTrianglesUVWs(void) const;
    const GLfloat* dataPointPositions(void) const;
    int countPointPositions(void) const;
    const GLfloat* dataLinePositions(void) const;
    int countLinePositions(void) const;
    QMatrix4x4 centeringTransform(void);
    QMatrix4x4 rangeScalingTransform(void);
    QVector3D boundsMin(void);
    QVector3D boundsMax(void);
    void dumpPoints(void);
  private:
    void updateBounds(QVector3D);
    void performDerivative(Region&);
    Region performSplit(Region&);
    void pushRegionToPoints(const Region&);
    std::vector<Region> m_regions_heap;
    std::vector<Region> m_small_regions;
    QVector<GLfloat> m_pts;
    QVector<GLfloat> m_verts;
    QVector<GLfloat> m_normals;
    QVector<GLfloat> m_uvs;
    QVector<GLfloat> m_lines;
//    void (*m_fn)(const QVector<double>&, double&);
    AdaptorInterface* m_adpt;
    const QVector<double>& m_orig_x;
    int m_x_index;
    int m_y_index;
    int m_max_regions = 512;
    GLfloat m_area_threshold = 0.03f;
    GLfloat m_max_aspect = 2;
    QVector3D m_min = {0,0,0};
    QVector3D m_max = {0,0,0};
  signals:
    void pointDataReady(void);
  public slots:
};

#endif // ADAPTIVEMESH_H
