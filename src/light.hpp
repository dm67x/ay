#pragma once

#include "math.hpp"
#include "color.hpp"
#include "object.hpp"

struct Light {
    Vec3 position;
    Color color;
    float power;

    Light() : position(0.f, 0.f, 0.f), color(0xffffffff), power(1.f) {}
};