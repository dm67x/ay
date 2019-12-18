#pragma once

#include "Scene/Entity.hpp"

#include <glm/glm.hpp>

struct Light : public SceneEntity
{
    glm::vec3 position = glm::vec3(0);
    glm::vec3 color = glm::vec3(1);
    float power = 10.f;

public:
    virtual void draw(const ShaderProgram&) const override;
};