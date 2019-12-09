#pragma once

#include "timer.h"
#include "ay.h"

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
    AY_API explicit Device(GLuint, GLuint);
    AY_API ~Device();

    AY_API bool run();
    AY_API void resize(GLuint, GLuint);
    AY_API void clear(GLbitfield);

    inline AY_API SDL_Window* window() const { return m_window; }
    inline AY_API Timer systemClock() const { return m_sysClock; }
    inline AY_API GLuint width() const { return m_width; }
    inline AY_API GLuint height() const { return m_height; }
};