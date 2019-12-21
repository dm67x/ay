#pragma once

#include <cstdint>

class Timer
{
    double m_oldTime;
    double m_elapsedTime;

public:
    Timer();
    void update();

    inline double elapsedTime() const { return m_elapsedTime; }
};