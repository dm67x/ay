#pragma once

#include <SDL.h>

enum EventType
{
    KEYDOWN,
    KEYUP,
    MOUSEBUTTONDOWN,
    MOUSEBUTTONUP,
    MOUSEMOTION
};

template<typename T>
struct Event
{
    EventType type;
    T event;
};

using KeyEvent = Event<SDL_KeyboardEvent>;
using MouseEvent = Event<SDL_MouseEvent>;