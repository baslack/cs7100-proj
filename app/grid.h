#ifndef GRID_H
#define GRID_H

#include <qopengl.h>
#include <QVector>

class grid {
  public:
    grid(GLfloat, int);
    //    GLfloat* getPointData(void);
    //    int getPointCount(void);
    const GLfloat* getLineData(void) const;
    int getLineCount(void) const;
  private:
    //    QVector<GLfloat> m_data_pts;
    QVector<GLfloat> m_data_lines;
};

#endif // GRID_H
