#include "window.hpp"
#include "platform/opengl.hpp"
#include <iostream>
#include <cstdlib>

void createWindow(int width, int height) {
    if (!glfwInit()) {
        std::cerr << "cannot init GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Ay", nullptr, nullptr);
    if (!window) {
        std::cerr << "cannot create window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!OpenGL::loadContext()) {
        std::cerr << "cannot create OpenGL context" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    std::cout << OpenGL::getVersion() << std::endl << OpenGL::getVendor() << std::endl;
}

void destroyWindow() {
    glfwDestroyWindow(getWindow());
    glfwTerminate();
}

GLFWwindow* getWindow() {
    return glfwGetCurrentContext();
}

bool windowIsOpen() {
    GLFWwindow* window = getWindow();
    glfwPollEvents();
    glfwSwapBuffers(window);
    return !glfwWindowShouldClose(window);
}