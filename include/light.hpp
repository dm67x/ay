#pragma once

#include "types.hpp"
#include "color.hpp"
#include <glm/glm.hpp>

class Light {
public:
    ///
    /// @brief Constructor
    ///
    Light() 
        : position(0.f, 0.f, 0.f), 
        color(0xffffffff), 
        intensity(1.f)
    {
    }

public:
    glm::vec3 position;
    Color color;
    f32 intensity;
};