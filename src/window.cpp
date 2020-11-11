#include "window.hpp"
#include "context.hpp"
#include <iostream>
#include <cstdlib>

Window::Window(int width, int height) : window(nullptr), ctx(nullptr) {
    if (!glfwInit()) {
        std::cerr << "cannot init GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Ay", nullptr, nullptr);
    if (!window) {
        std::cerr << "cannot create window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ctx = new Context();
    std::cout << ctx->getVersion() << std::endl << ctx->getVendor() << std::endl;
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

Context* Window::getCtx() const {
    return ctx;
}