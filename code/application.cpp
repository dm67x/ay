#include "application.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iostream>

Application::Application()
    : m_window{ nullptr },
    m_sysClock{}
{
    if (!glfwInit()) {
        throw std::runtime_error("cannot initialized GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    m_window = glfwCreateWindow(800, 800, "", nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("cannot create window");
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("cannot load glad");
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

Application::~Application()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Application::run()
{
    m_sysClock.update();

    if (glfwWindowShouldClose(m_window))
        return false;

    glfwPollEvents();

    auto wsize = size();
    glViewport(0, 0, wsize.first, wsize.second);

    // window title
    std::stringstream title;
    title << "OpenGL ";
    title << glGetString(GL_VERSION);
    title << " [" << std::fixed << std::setprecision(2);
    title << m_sysClock.elapsedTime() * 1000.0 << "ms]";
    glfwSetWindowTitle(m_window, title.str().c_str());

    glfwSwapBuffers(m_window);
    return true;
}

void Application::resize(int w, int h)
{
    glfwSetWindowSize(m_window, w, h);
}

std::pair<int, int> Application::size() const
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return std::make_pair(width, height);
}

void Application::clear(GLbitfield mask) const
{
    glClear(mask);
}