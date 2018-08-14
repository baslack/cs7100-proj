#ifndef ADAPTIVEMESH_H
#define ADAPTIVEMESH_H

#include <QObject>
#include "geometry_interface.h"
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
        void (*)(const QVector<double>&, double&),
        const QVector<double>&,
        int,
        int,
        QVector2D,
        QVector2D,
        int,
        QObject* parent = 0
    );
    void clear(void) = 0;
    const GLfloat* dataTrianglesPositions(void) const = 0;
    int countTrianglesPositions(void) const = 0;
    const GLfloat* dataTrianglesNormals(void) const = 0;
    int countTrianglesNormals(void) const = 0;
    const GLfloat* dataTrianglesUVWs(void) const = 0;
    int countTrianglesUVWs(void) const = 0;
    const GLfloat* dataPointPositions(void) const = 0;
    int countPointPositions(void) const = 0;
    QMatrix4x4 centeringTransform(void) = 0;
    QMatrix4x4 rangeScalingTransform(void) = 0;
    QVector3D boundsMin(void)=0;
    QVector3D boundsMax(void)=0;
  private:
    void updateBounds(QVector3D);
    void performDerivative(Region&);
    Region performSplit(Region&);
    void pushRegionToPoints(const Region&);
    std::vector<Region> m_regions_heap;
    QVector<GLfloat> m_pts;
    void (*m_fn)(const QVector<double>&, double&);
    const QVector<double>& m_orig_x;
    int m_x_index;
    int m_y_index;
    int m_max_regions;
    QVector3D m_min = {0,0,0};
    QVector3D m_max = {0,0,0};
  signals:

  public slots:
};

#endif // ADAPTIVEMESH_H
