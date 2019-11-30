#include "device.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>

Device::Device(GLuint width, GLuint height)
    : m_window{ nullptr },
    m_deltaTime{ 0 },
    m_width{ width },
    m_height{ height }
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

bool Device::run()
{
    double oldTime = glfwGetTime();
    double currentTime = 0;

    if (glfwWindowShouldClose(m_window))
        return false;

    glfwPollEvents();

    // ips calculation
    currentTime = glfwGetTime();
    m_deltaTime = currentTime - oldTime;
    oldTime = currentTime;

    // window title
    std::stringstream title;
    title << "OpenGL ";
    title << glGetString(GL_VERSION);
    title << " [" << std::fixed << std::setprecision(2);
    title << m_deltaTime * 1000.f << "ms]";
    glfwSetWindowTitle(m_window, title.str().c_str());

    glViewport(0, 0, m_width, m_height);

    glfwSwapBuffers(m_window);
    
    return true;
}

void Device::resize(GLuint w, GLuint h)
{
    m_width = w;
    m_height = h;
}