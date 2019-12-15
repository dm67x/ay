#pragma once

#include "Scene/Entity.hpp"
#include "Ay.hpp"

#include <glm/glm.hpp>

struct Light : public SceneEntity
{
    glm::vec3 position;

    virtual void draw() const = 0;
};

struct PointLight final : public Light
{
    AY_API void draw() const override;
};

struct DirectionalLight final : public Light
{
    AY_API void draw() const override;
};