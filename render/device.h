#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

class Device final
{
    GLFWwindow* m_window;
    double m_deltaTime;
    GLuint m_width;
    GLuint m_height;

public:
    Device(GLuint, GLuint);
    ~Device();

    bool run();
    void resize(GLuint, GLuint);

    inline GLFWwindow* window() const { return m_window; }
    inline double deltaTime() const { return m_deltaTime; }
    inline GLuint width() const { return m_width; }
    inline GLuint height() const { return m_height; }
};