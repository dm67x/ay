#pragma once

#include "shader.h"

#include <vector>
#include <memory>

struct Texture2D;

struct Material
{
    Material();
    virtual ~Material() = default;

    void Diffuse(const std::shared_ptr<Texture2D>&);
    void Specular(const std::shared_ptr<Texture2D>&);
    void Use(const ShaderProgram&) const;

private:
    std::vector<std::shared_ptr<Texture2D>> m_diffuseTextures;
    std::vector<std::shared_ptr<Texture2D>> m_specularTextures;
};