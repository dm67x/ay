#include "device.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>

Device::Device(GLuint width, GLuint height)
    : m_window{ nullptr }
{
    if (!glfwInit()) {
        throw std::runtime_error("cannot initialized glfw");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    m_window = glfwCreateWindow(
        width,
        height,
        "",
        nullptr,
        nullptr
    );

    if (!m_window) {
        throw std::runtime_error("cannot create window");
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("cannot load glad");
    }
}

Device::~Device()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Device::Run()
{
    double oldTime = glfwGetTime();
    double currentTime = 0;
    double deltaTime = 0;

    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        // ips calculation
        currentTime = glfwGetTime();
        deltaTime = currentTime - oldTime;
        oldTime = currentTime;

        // window title
        std::stringstream title;
        title << "OpenGL ";
        title << glGetString(GL_VERSION);
        title << " [" << std::fixed << std::setprecision(2);
        title << deltaTime * 1000.f << "ms]";
        glfwSetWindowTitle(m_window, title.str().c_str());

        glfwSwapBuffers(m_window);
    }
}