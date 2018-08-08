#ifndef PRIMATIVES_H
#define PRIMATIVES_H

#include <QVector>
#include <QVector3D>
#include <qopengl.h>

class Point;
class Vertex;
class HalfEdge;
class Face;

class Point {
  public:
    Point(QVector3D);
    Point(GLfloat, GLfloat, GLfloat);
    const QVector3D getPosition(void) const;
    void setPosition(QVector3D);
    void setPosition(GLfloat, GLfloat, GLfloat);
  private:
    QVector3D m_position;
};

class Vertex {
  public:
    Vertex(void);
    ~Vertex(void);
    Vertex(Point*, HalfEdge*);
    Point* getPoint(void);
    HalfEdge* getEdge(void);
    QVector3D getPosition(void);
    QVector3D getNormal(void);
    QVector3D getUVW(void);
    void setPoint(Point*);
    void setEdge(HalfEdge*);
    void setPosition(QVector3D);
    void setNormal(QVector3D);
    void setUVW(QVector3D);
  private:
    Point* m_point;
    HalfEdge* m_arb_edge;
    QVector<QVector3D> m_data;
};

class HalfEdge{
  public:
    HalfEdge(void);
    ~HalfEdge(void);
    HalfEdge(Vertex*, Face*, HalfEdge*, HalfEdge*, HalfEdge*);
    const HalfEdge* next(void);
    const HalfEdge* prev(void);
    const HalfEdge* twin(void);
    const Face* face(void);
    const Vertex* start(void);
    void setNext(HalfEdge*);
    void setPrev(HalfEdge*);
    void setTwin(HalfEdge*);
    void setFace(Face*);
    void setStart(Vertex*);
private:
    HalfEdge* m_next;
    HalfEdge* m_prev;
    HalfEdge* m_twin;
    Face* m_face;
    Vertex* m_start;
};

class Face {
  public:
    Face(void);
    Face(HalfEdge*);
  private:
    HalfEdge* m_start_edge;
};


#endif // PRIMATIVES_H
