#include "window.hpp"
#include "context.hpp"
#include <iostream>
#include <cstdlib>

static void glfw_error_callback(int error, const char* description) {
    spdlog::critical("Error [{}]: {}", error, description);
}

Window::Window(i32 width, i32 height) 
    : window(nullptr), 
    ctx(nullptr), 
    cursorVisible(false), 
    cursorProcessed(false) 
{
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

bool Window::isOpen() {
    glfwPollEvents();
    glfwSwapBuffers(window);

    if (isKeyPressed(GLFW_KEY_F1) && !cursorProcessed) {
        cursorVisible = !cursorVisible;
        if (cursorVisible) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        cursorProcessed = true;
    }

    if (isKeyReleased(GLFW_KEY_F1)) {
        cursorProcessed = false;
    }

    if (isKeyPressed(GLFW_KEY_ESCAPE)) {
        close();
    }

    return !glfwWindowShouldClose(window);
}