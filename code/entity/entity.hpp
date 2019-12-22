#pragma once

#include "model/transform.hpp"

class Shader;

struct Entity : public Transform
{
    virtual void draw(const Shader&) const = 0;
};