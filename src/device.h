#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Device final
{
    GLFWwindow* m_window;

public:
    Device(GLuint, GLuint);
    ~Device();

    void Run();
};