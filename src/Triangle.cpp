#include "pch.h"
#include "Triangle.h"


Triangle::Triangle(float x, float y, float width, float height) {
    std::array<float, 6> modelData {
        x,               y,
        x + width,       y,
        x + (width / 2), y + height,
    };

    std::array<float, 9> colorData {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    m_array.bind();
    m_model.usage(lwvl::Usage::Static);
    m_model.construct(modelData.begin(), modelData.end());

    m_color.usage(lwvl::Usage::Static);
    m_color.construct(colorData.begin(), colorData.end());

    m_model.bind();
    m_array.attribute(2, GL_FLOAT, 2 * sizeof(float), 0);

    m_color.bind();
    m_array.attribute(3, GL_FLOAT, 3 * sizeof(float), 0);
    m_array.clear();
}

void Triangle::draw() {
    m_array.bind();
    m_array.drawArrays(GL_TRIANGLES, 3);
    m_array.clear();
}
