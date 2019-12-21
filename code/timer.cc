#include "timer.h"

#include <GLFW/glfw3.h>

Timer::Timer()
    : m_oldTime{ 0 },
    m_elapsedTime{ 0 }
{
}

void Timer::update()
{
    const double currentTime = glfwGetTime();
    const double deltaTime =  currentTime - m_oldTime;
    m_oldTime = currentTime;
    m_elapsedTime = deltaTime;
}