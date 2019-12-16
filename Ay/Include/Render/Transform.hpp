#pragma once

#include "Ay.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
    glm::vec3 position = glm::vec3(0);
    glm::vec3 rAxis = glm::vec3(0, 1, 0);
    float rAmount = 0;
    glm::vec3 size = glm::vec3(1);

public:
    AY_API void translate(const glm::vec3& position)
    {
        this->position = position;
    }

    AY_API void rotate(float angle, const glm::vec3& axis)
    {
        rAmount = angle;
        rAxis = axis;
    }

    AY_API void scale(const glm::vec3& scale)
    {
        size = scale;
    }

    AY_API glm::mat4 transform() const
    {
        auto translate = glm::translate(glm::mat4(1.f), position);
        auto rotate = glm::rotate(glm::mat4(1.f), rAmount, rAxis);
        auto scaling = glm::scale(glm::mat4(1.f), size);
        return scaling * rotate * translate;
    }
};