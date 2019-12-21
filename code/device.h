#pragma once

#include "timer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <tuple>

class Device final
{
    GLFWwindow* m_window;
    Timer m_sysClock;

public:
    Device(GLuint, GLuint);
    ~Device();

public:
    bool run();
    void resize(int, int);
    void clear(GLbitfield) const;
    std::pair<int, int> size() const;

public:
    inline GLFWwindow* window() const { return m_window; }
    inline Timer systemClock() const { return m_sysClock; }
};