#include "device.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iostream>

Device::Device(GLuint width, GLuint height)
    : m_window{ nullptr },
    m_context{ },
    m_sysClock{},
    m_width{ width },
    m_height{ height },
    m_keyEventFunc{ [](Uint32, SDL_KeyboardEvent) {} },
    m_mouseEventFunc{ [](Uint32) {} }
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("cannot initialized SDL2");
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    m_window = SDL_CreateWindow("",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!m_window) {
        throw std::runtime_error("cannot create window");
    }

    m_context = SDL_GL_CreateContext(m_window);
    SDL_GL_SetSwapInterval(1);

    SDL_GL_MakeCurrent(m_window, m_context);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        throw std::runtime_error("cannot load glad");
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

Device::~Device()
{
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Device::run()
{
    m_sysClock.update();

    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            return false;
        }
        else if (evt.type == SDL_WINDOWEVENT) {
            if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                resize(evt.window.data1, evt.window.data2);
            }
        }
        else if (evt.type == SDL_KEYDOWN || evt.type == SDL_KEYUP) {
            m_keyEventFunc(evt.type, evt.key);
        }
        else if (evt.type == SDL_MOUSEBUTTONDOWN 
            || evt.type == SDL_MOUSEBUTTONUP
            || evt.type == SDL_MOUSEMOTION) 
        {
            m_mouseEventFunc(evt.type);
        }
    }

    glViewport(0, 0, m_width, m_height);

    // window title
    std::stringstream title;
    title << "OpenGL ";
    title << glGetString(GL_VERSION);
    title << " [" << std::fixed << std::setprecision(2);
    title << m_sysClock.elapsedTime() * 1000.0 << "ms]";
    SDL_SetWindowTitle(m_window, title.str().c_str());

    SDL_GL_SwapWindow(m_window);
    return true;
}

void Device::resize(GLuint w, GLuint h)
{
    m_width = w;
    m_height = h;
}

void Device::clear(GLbitfield mask)
{
    glClear(mask);
}