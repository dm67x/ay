#include "window.hpp"
#include "context.hpp"
#include <iostream>
#include <cstdlib>

static void glfw_error_callback(int error, const char* description) {
    spdlog::critical("Error [{}]: {}", error, description);
}

Window::Window(int width, int height) : window(nullptr), ctx(nullptr) {
    glfwSetErrorCallback(glfw_error_callback);
    
    if (!glfwInit()) {
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Ay", nullptr, nullptr);
    if (!window) {
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ctx = new Context(*this);
    spdlog::info("{}", ctx->getVersion());
    spdlog::info("{}", ctx->getVendor());
}

Window::~Window() {
    delete ctx;
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::isOpen() const {
    glfwPollEvents();
    glfwSwapBuffers(window);
    return !glfwWindowShouldClose(window);
}