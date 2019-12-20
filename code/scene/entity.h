#pragma once

#include "render/transform.h"
#include "render/shader.h"

class SceneEntity : public Transform
{
public:
    virtual void draw(const Shader&) const = 0;
};