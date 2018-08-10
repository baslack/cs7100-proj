#include "primatives.h"

Point::Point(QVector3D ws_pos):
    m_position(ws_pos) {

}

const QVector3D Point::getPosition(void) const{
    return m_position;
}

void Point::setPosition(QVector3D ws_pos){
    m_position = ws_pos;
}

void Point::setPosition(GLfloat x, GLfloat y, GLfloat z){
    m_position = QVector3D(x, y, z);
}
