#pragma once

#include "color.hpp"
#include "object.hpp"
#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;
    Color color;
    float intensity;

    Light() 
        : position(0.f, 0.f, 0.f), 
        color(0xffffffff), 
        intensity(1.f)
    {
    }
};