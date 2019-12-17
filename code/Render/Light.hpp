#pragma once

#include "Scene/Entity.hpp"

#include <glm/glm.hpp>

struct Light : public SceneEntity
{
    glm::vec3 position;

public:
    virtual void draw(const ShaderProgram&) const override;
};