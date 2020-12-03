#include "pch.h"
#include "Core/Utils.h"
#include "Core/Simulation.h"
#include "Renderer/GLDebug.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

using namespace lwvl;


// Left-most-point-defined iscoceles triangle.
class Triangle {
    class VertexArray {
        unsigned int attributes = 0;
    public:
        unsigned int id;

        VertexArray() {
            glGenVertexArrays(1, &id);
        }

        ~VertexArray() {
            glDeleteVertexArrays(1, &id);
        }

        void bind() {
            glBindVertexArray(id);
        }

        static void clear() {
            glBindVertexArray(0);
        }

        void attribute(uint8_t dimensions, GLenum type, int64_t stride, int64_t offset) {
            glEnableVertexAttribArray(attributes);
            glVertexAttribPointer(attributes, dimensions, type, GL_FALSE, stride, reinterpret_cast<void*>(offset));
            attributes++;
        }

        void drawArrays(GLenum mode, int count) {
            glDrawArrays(mode, 0, count);
        }
    };

    ShaderProgram m_program;
    VertexArray m_array;
    ArrayBuffer m_model;

    public:
    Triangle(float x, float y, float width, float height) {
        std::array<float, 15> modelData {
            x,               y,          1.0f, 0.0f, 0.0f,
            x + width,       y,          0.0f, 1.0f, 0.0f,
            x + (width / 2), y + height, 0.0f, 0.0f, 1.0f
        };

        m_program.link(readFile("Shaders/triangle.vert"), readFile("Shaders/triangle.frag"));

        m_array.bind();
        m_model.usage(Usage::Static);
        m_model.construct(modelData.begin(), modelData.end());

        m_model.bind();
        m_array.attribute(2, GL_FLOAT, 5 * sizeof(float), 0);
        m_array.attribute(3, GL_FLOAT, 5 * sizeof(float), 2 * sizeof(float));

        m_array.clear();
        m_model.clear();
    }

    void draw() {
        m_program.bind();
        m_array.bind();
        m_array.drawArrays(GL_TRIANGLES, 3);
        m_array.clear();
        m_program.clear();
    }
};


class HelloTriangle : public Simulation {
    static void gl_callback(
        debug::Source source, debug::Type type, unsigned int id,
        debug::Severity severity, int length, const char* message,
        const void* userState
    ) {
        if (type == debug::Type::ERROR) {
            throw std::exception(message);
        } else {
#ifndef NDEBUG
            std::cout << "[OpenGL] " << message << std::endl;
#endif // !NDEBUG
        }
    }

    debug::GLEventListener listener = debug::GLEventListener(this, gl_callback);
    Triangle triangle;

    public:
    HelloTriangle(unsigned int width, unsigned int height, GLFWmonitor* monitor = nullptr): 
        Simulation(width, height, "HelloTriangle", monitor), triangle(-0.5f, -0.5f, 1.0f, 1.0f)
    {}

    void draw() final {
        glClear(GL_COLOR_BUFFER_BIT);
        triangle.draw();
    }
};


int main() {
    try {
        HelloTriangle app(800, 600);
        app.run();
        return 0;
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
}
