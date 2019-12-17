#pragma once

#include "Scene/Entity.hpp"
#include "Ay.hpp"

#include <glm/glm.hpp>

struct Light : public SceneEntity
{
    glm::vec3 position;

    AY_API virtual void draw(const ShaderProgram&) const override;
};