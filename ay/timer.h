#pragma once

#include "ay.h"

#include <cstdint>

class Timer
{
    uint64_t m_oldTime;
    double m_elapsedTime;
    static const uint64_t TICKS_PER_SECONDS;

public:
    AY_API Timer();
    AY_API void update();

    inline AY_API double elapsedTime() const { return m_elapsedTime; }
};