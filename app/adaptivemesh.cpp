#include "adaptivemesh.h"
#include <algorithm>

#define H_RATIO 8

Region::Region(QVector2D min, QVector2D max):
    m_min(min),
    m_max(max),
    m_max_on_X(false),
    m_max_derivative(0) {

}

QVector2D Region::getMidpoint() const {
    return (m_max - m_min) / 2 + m_min;
}

QVector2D Region::getMinpoint() const {
    return m_min;
}

void Region::setMinpoint(QVector2D min) {
    m_min = min;
}

QVector2D Region::getMaxpoint() const {
    return m_max;
}

void Region::setMaxpoint(QVector2D max) {
    m_max = max;
}

double Region::getMaxDerivative() const {
    return m_max_derivative;
}

bool Region::getMaxOnX() const {
    return m_max_on_X;
}

void Region::setMaxDerivative(bool max_on_X, double max_derivative) {
    m_max_on_X = max_on_X;
    m_max_derivative = max_derivative;
}

bool Region::operator<(const Region& other) const {
    return (this->m_max_derivative < other.m_max_derivative);
}

bool Region::Compare(const Region& lhs, const Region& rhs) {
    return (lhs.m_max_derivative < rhs.m_max_derivative);
}

AdaptiveMesh::AdaptiveMesh(
    void (*fn)(const QVector<double>&, double&),
    const QVector<double>& orig_x,
    int x_index,
    int y_index,
    QVector2D min_bound,
    QVector2D max_bound,
    int max_regions,
    QObject* parent)
    :
    QObject(parent),
    m_fn(fn),
    m_orig_x(orig_x),
    m_x_index(x_index),
    m_y_index(y_index),
    m_max_regions(max_regions) {

    // setup the first region
    Region start(min_bound, max_bound);
    performDerivative(start);

    // init the heap
    std::make_heap(m_regions_heap.begin(), m_regions_heap.end());
    m_regions_heap.push_back(start);
    std::push_heap(m_regions_heap.begin(), m_regions_heap.end());
    while (int(m_regions_heap.size()) < max_regions) {
        // pop top heap
        std::pop_heap(m_regions_heap.begin(), m_regions_heap.end());
        Region popped = m_regions_heap.back();
        m_regions_heap.pop_back();
        // split
        Region new_region = performSplit(popped);
        // perform derivative
        performDerivative(popped);
        performDerivative(new_region);
        // add both back to heap
        m_regions_heap.push_back(popped);
        std::push_heap(m_regions_heap.begin(), m_regions_heap.end());
        m_regions_heap.push_back(new_region);
        std::push_heap(m_regions_heap.begin(), m_regions_heap.end());
    }
    // dump the regions points and results to the point array
    for (auto this_region : m_regions_heap) {
        pushRegionToPoints(this_region);
    }
}

void AdaptiveMesh::performDerivative(Region& this_region) {
    // setup the offsets for the stencil
    QVector2D max = this_region.getMaxpoint();
    QVector2D min = this_region.getMinpoint();
    double h_x = max.x() - min.x();
    h_x /= H_RATIO;
    double h_y = max.y() - min.y();
    h_y /= H_RATIO;
    QVector<double> scratch_x(m_orig_x);
    QVector2D mid = this_region.getMidpoint();
    scratch_x[m_x_index] = mid.x();
    scratch_x[m_y_index] = mid.y();
    double dzdx = fourth_derivative(m_fn, scratch_x, m_x_index, h_x);
    double dzdy = fourth_derivative(m_fn, scratch_x, m_y_index, h_y);
    if (dzdx >= dzdy) {
        this_region.setMaxDerivative(true, dzdx);
    } else {
        this_region.setMaxDerivative(false, dzdy);
    }
}

Region AdaptiveMesh::performSplit(Region& old_region) {
    QVector2D old_min = old_region.getMinpoint();
    QVector2D old_max = old_region.getMaxpoint();
    QVector2D old_mid = old_region.getMidpoint();
    QVector2D new_min, new_max;
    if (old_region.getMaxOnX()) {
        new_min.setX(old_mid.x());
        new_min.setY(old_min.y());
        new_max.setX(old_max.x());
        new_max.setY(old_max.y());
        old_max.setX(old_mid.x());
    } else {
        new_min.setX(old_min.x());
        new_min.setY(old_mid.y());
        new_max.setX(old_max.x());
        new_max.setY(old_max.y());
        old_max.setY(old_mid.y());
    }
    return Region(new_min, new_max);
}

void AdaptiveMesh::pushRegionToPoints(const Region& this_region) {
    QVector2D min = this_region.getMinpoint();
    QVector2D max = this_region.getMaxpoint();
    QVector<QVector2D>pts;
    pts.push_back(min);
    pts.push_back(QVector2D(max.x(), min.y()));
    pts.push_back(max);
    pts.push_back(QVector2D(min.x(), max.y()));
    double z = 0;
    QVector<double>scratch_x(m_orig_x);
    for (auto this_pt : pts) {
        scratch_x[m_x_index] = this_pt.x();
        scratch_x[m_y_index] = this_pt.y();
        m_fn(scratch_x, z);
        m_pts.push_back(this_pt.x());
        m_pts.push_back(this_pt.y());
        m_pts.push_back(GLfloat(z));
        updateBounds(QVector3D(this_pt.x(), this_pt.y(), GLfloat(z)));
    }
}

void AdaptiveMesh::clear(void) {
    m_regions_heap.clear();
}

const GLfloat* AdaptiveMesh::dataTrianglesPositions(void)const {
    return Q_NULLPTR;
}

int AdaptiveMesh::countTrianglesPositions(void) const {
    return -1;
}

const GLfloat* AdaptiveMesh::dataTrianglesNormals(void) const {
    return Q_NULLPTR;
}

int AdaptiveMesh::countTrianglesNormals(void) const {
    return -1;
}

const GLfloat* AdaptiveMesh::dataTrianglesUVWs(void) const {
    return Q_NULLPTR;
}

int AdaptiveMesh::countTrianglesUVWs(void) const {
    return -1;
}

const GLfloat* AdaptiveMesh::dataPointPositions(void) const {
    return m_pts.constData();
}

int AdaptiveMesh::countPointPositions(void) const {
    return m_pts.size();
}

QMatrix4x4 AdaptiveMesh::centeringTransform(void)  {
    QMatrix4x4 retVal;
    retVal.setToIdentity();
    QVector3D offset = m_max - m_min;
    offset *= -1;
    retVal.translate(offset);
    return retVal;
}

QMatrix4x4 AdaptiveMesh::rangeScalingTransform(void) {
    QMatrix4x4 retVal;
    retVal.setToIdentity();
    QVector3D range = m_max - m_min;
    GLfloat z_scale = 1;
    if (range.z() / range.x() >= range.z() / range.y() ) {
        z_scale = range.y() / range.z();
    }else{
        z_scale = range.x() / range.z();
    }
    retVal.scale(1,1,z_scale);
    return retVal;
}

QVector3D AdaptiveMesh::boundsMin(void) {
    return m_min;
}

QVector3D AdaptiveMesh::boundsMax(void) {
    return m_max;
}

void AdaptiveMesh::updateBounds(QVector3D new_pt) {
    if (new_pt.x() < m_min.x()) {
        m_min.setX(new_pt.x());
    } else if(new_pt.x() > m_max.x()) {
        m_max.setX(new_pt.x());
    }
    if (new_pt.y() < m_min.y()) {
        m_min.setY(new_pt.y());
    } else if(new_pt.y() > m_max.y()) {
        m_max.setY(new_pt.y());
    }
    if (new_pt.z() < m_min.z()) {
        m_min.setZ(new_pt.z());
    } else if(new_pt.z() > m_max.z()) {
        m_max.setZ(new_pt.z());
    }
}
