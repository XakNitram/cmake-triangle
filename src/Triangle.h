#include "pch.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"


class Triangle {
    lwvl::VertexArray m_array;
    lwvl::ArrayBuffer m_model;

public:
    Triangle(float x, float y, float width, float height);
    void draw();
};
