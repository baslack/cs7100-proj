#include "grid.h"

grid::grid(GLfloat size, int steps) {
    GLfloat line_len = 2 * size;
    GLfloat step = line_len / steps;
    for (int i = 0; i < steps + 1; i++) {
        m_data_lines.push_back(i * step - size);
        m_data_lines.push_back(-size);
        m_data_lines.push_back(i * step - size);
        m_data_lines.push_back(size);
        if (i == 5) {
            m_data_lines.push_back(1.0f);
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(0.0f);
        } else {
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
        }
    }
    for (int j = 0; j < steps; j++) {
        m_data_lines.push_back(-size);
        m_data_lines.push_back(j * step - size);
        m_data_lines.push_back(size);
        m_data_lines.push_back(j * step - size);
        if (j == 5) {
            m_data_lines.push_back(0.0f);
            m_data_lines.push_back(1.0f);
            m_data_lines.push_back(0.0f);
        } else {
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
            m_data_lines.push_back(0.3f);
        }
    }
}

const GLfloat* grid::getLineData() const{
    return m_data_lines.constData();
}

int grid::getLineCount() const{
    return m_data_lines.size();
}
