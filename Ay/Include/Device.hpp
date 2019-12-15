#pragma once

#include "Timer.hpp"
#include "Ay.hpp"

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
    std::function<void(Uint32, SDL_KeyboardEvent)> m_keyEventFunc;
    std::function<void(Uint32)> m_mouseEventFunc;

public:
    AY_API Device(GLuint, GLuint);
    AY_API ~Device();

    AY_API bool run();
    AY_API void resize(GLuint, GLuint);
    AY_API void clear(GLbitfield);

    inline SDL_Window* window() const { return m_window; }
    inline Timer systemClock() const { return m_sysClock; }
    inline GLuint width() const { return m_width; }
    inline GLuint height() const { return m_height; }
};