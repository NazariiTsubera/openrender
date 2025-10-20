#include <iostream>
#include <array>
#include <GLFW/glfw3.h>

#include "openrenderer/renderer.hpp"



int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    oren::DeviceDesc device{oren::Backend::OpenGL, nullptr};
    oren::Renderer renderer(device);

    float data[] = {
         0.0f,  0.6f, 0.0f,
        -0.6f, -0.6f, 0.0f,
         0.6f, -0.6f, 0.0f
    };

    oren::BufferHandle vb = renderer.create_buffer({
        data,
        sizeof(data),
        oren::BufferType::VERTEX
    });

    oren::VertexAttribDesc attr{
        /*handle*/  vb,
        /*location*/0,
        /*format*/  oren::AttribFormat::r32g32b32_float,
        /*stride*/  3u * sizeof(float),
        /*offset*/  0u,
        /*divisor*/ 0u
    };
    oren::IndexDesc no_idx{};

    oren::VertexInputHandle vi =
        renderer.create_vertex_input(std::span<const oren::VertexAttribDesc>(&attr, 1), no_idx);

    const char* VS = R"(#version 330 core
        layout(location=0) in vec3 aPos;
        void main() { gl_Position = vec4(aPos, 1.0); }
        )";
    const char* FS = R"(#version 330 core
        out vec4 FragColor;
        void main() { FragColor = vec4(0.95, 0.3, 0.45, 1.0); }
        )";
    oren::ShaderHandle prog = renderer.create_shader({ VS, FS });

    oren::ColorDesc clear_col{0.1f, 0.1f, 0.12f, 1.0f};

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        renderer.begin_frame();
        renderer.clear_color(clear_col);
        renderer.clear();

        renderer.bind_shader(prog);

        renderer.draw(vi,3, 0);

        renderer.end_frame();

        glfwSwapBuffers(window);
    }



    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
