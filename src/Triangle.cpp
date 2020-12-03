#include "pch.h"
#include "Triangle.h"


Triangle::Triangle(float x, float y, float width, float height) {
    std::array<float, 15> modelData {
        x,               y,          1.0f, 0.0f, 0.0f,
        x + width,       y,          0.0f, 1.0f, 0.0f,
        x + (width / 2), y + height, 0.0f, 0.0f, 1.0f
    };

    m_array.bind();
    m_model.usage(lwvl::Usage::Static);
    m_model.construct(modelData.begin(), modelData.end());

    m_model.bind();
    m_array.attribute(2, GL_FLOAT, 5 * sizeof(float), 0);
    m_array.attribute(3, GL_FLOAT, 5 * sizeof(float), 2 * sizeof(float));

    m_array.clear();
    m_model.clear();
}

void Triangle::draw() {
    m_array.bind();
    m_array.drawArrays(GL_TRIANGLES, 3);
    m_array.clear();
}
