#include "pch.h"
#include "Core/Utils.h"
#include "Core/Simulation.h"
#include "Renderer/GLDebug.h"
#include "Renderer/Shader.h"
#include "Triangle.h"

using namespace lwvl;


// Left-most-point-defined iscoceles triangle.


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
    ShaderProgram program;
    Triangle triangle;

public:
    HelloTriangle(unsigned int width, unsigned int height, GLFWmonitor* monitor = nullptr): 
        Simulation(width, height, "HelloTriangle", monitor), 
        triangle(-0.5f, -0.5f, 1.0f, 1.0f)
    {
        program.link(readFile("Shaders/triangle.vert"), readFile("Shaders/triangle.frag"));
        swapInterval(1);
    }

    void draw() final {
        glClear(GL_COLOR_BUFFER_BIT);
        program.bind();
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
