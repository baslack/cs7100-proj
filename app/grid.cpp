#include "grid.h"

grid::grid(GLfloat size, int steps) {
    GLfloat line_len = 2 * size;
    GLfloat step = line_len / steps;
    for (int i = 0; i < steps + 1; i++) {
        if (i != 5){
            // left point
            m_data_lines.push_back(i * step - size);
            m_data_lines.push_back(-size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
            //right point
            m_data_lines.push_back(i * step - size);
            m_data_lines.push_back(size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
        }else{
            // split the axis line, light and dark
            // dark half
            m_data_lines.push_back(i * step - size);
            m_data_lines.push_back(-size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.5f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(i * step - size);
            m_data_lines.push_back(0);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.5f);
            m_data_lines.push_back(0.0f);
            // light half
            m_data_lines.push_back(i * step - size);
            m_data_lines.push_back(0);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(1.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(i * step - size);
            m_data_lines.push_back(size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(1.0f);
            m_data_lines.push_back(0.0f);
        }
    }
    for (int j = 0; j < steps + 1; j++) {
        if (j != 5){
            m_data_lines.push_back(-size);
            m_data_lines.push_back(j * step - size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(size);
            m_data_lines.push_back(j * step - size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
        }else{
            // split the line into light and dark
            // dark
            m_data_lines.push_back(-size);
            m_data_lines.push_back(j * step - size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.5f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0);
            m_data_lines.push_back(j * step - size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.5f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0);
            m_data_lines.push_back(j * step - size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(1.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(size);
            m_data_lines.push_back(j * step - size);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(1.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.0f);
        }
    }
    // z-axis
    // dark
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(-size);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.5f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.5f);
    // light
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(1.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(size);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(0.0f);
    m_data_lines.push_back(1.0f);
}

const GLfloat* grid::getLineData() const{
    return m_data_lines.constData();
}

int grid::getLineCount() const{
    return m_data_lines.size();
}
