#include "window.h"

#include <stdio.h>
#include <stdlib.h>

ayWindow ayWindow_new(uint32_t width, uint32_t height, ayContext* context)
{
    ayWindow window;
    window.width = width;
    window.height = height;
    window.context = context;
    window.glwin = NULL;

    if (!glfwInit()) {
        // error message
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, window.context->major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, window.context->minor);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    window.glwin = glfwCreateWindow(
        window.width, window.height, 
        "", NULL, NULL);

    if (!window.glwin) {
        // error log
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window.glwin);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        // error log
        exit(EXIT_FAILURE);
    }

    return window;
}

void ayWindow_destroy(ayWindow* window)
{
    glfwDestroyWindow(window->glwin);
    glfwTerminate();
}

bool ayWindow_show(ayWindow* window)
{
    double oldTime = glfwGetTime();
    double currentTime = 0;
    double deltaTime = 0;

    if (glfwWindowShouldClose(window->glwin))
        return false;

    glfwPollEvents();

    // ips calculation
    currentTime = glfwGetTime();
    deltaTime = currentTime - oldTime;
    oldTime = currentTime;

    // window title
    char title[250];
    sprintf_s(title, 250, "OpenGL %s [%.2fms]", glGetString(GL_VERSION), deltaTime * 1000.f);
    glfwSetWindowTitle(window->glwin, title);

    glfwSwapBuffers(window->glwin);
    return true;
}