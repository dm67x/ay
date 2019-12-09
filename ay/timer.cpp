#include "timer.h"

#include <SDL.h>

const uint64_t Timer::TICKS_PER_SECONDS = SDL_GetPerformanceFrequency();

Timer::Timer()
    : m_oldTime{ 0 },
    m_elapsedTime{ 0 }
{
}

void Timer::update()
{
    const uint64_t currentTime = SDL_GetPerformanceCounter();
    const uint64_t deltaTime =  currentTime - m_oldTime;
    m_oldTime = currentTime;
    m_elapsedTime = deltaTime / static_cast<double>(TICKS_PER_SECONDS);
}