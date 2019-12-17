#pragma once

#include "Render/Transform.hpp"
#include "Render/Shader.hpp"

class SceneEntity : public Transform
{
public:
    virtual void draw(const ShaderProgram&) const = 0;
};