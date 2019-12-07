#pragma once

#include "main/timer.h"

#include <glad/glad.h>
#include <SDL.h>
#include <functional>

class Device final
{
    SDL_Window* m_window;
    SDL_GLContext m_context;
    Timer m_sysClock;
    GLuint m_width;
    GLuint m_height;

public:
    Device(GLuint, GLuint);
    ~Device();

    bool run();
    void resize(GLuint, GLuint);

    inline SDL_Window* window() const { return m_window; }
    inline Timer systemClock() const { return m_sysClock; }
    inline GLuint width() const { return m_width; }
    inline GLuint height() const { return m_height; }
};