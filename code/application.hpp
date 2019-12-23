#pragma once

#include "timer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <tuple>

class Application final
{
    GLFWwindow* m_window;
    Timer m_sysClock;

public:
    Application();
    ~Application();

public:
    bool run();
    void resize(int, int);
    void clear(GLbitfield) const;
    std::pair<int, int> size() const;

public:
    inline GLFWwindow* window() const { return m_window; }
    inline Timer systemClock() const { return m_sysClock; }
};