#pragma once

#include "shader.h"

#include <vector>
#include <memory>

class Texture2D;

class Material
{
    std::vector<std::shared_ptr<Texture2D>> m_diffuseTextures;
    std::vector<std::shared_ptr<Texture2D>> m_specularTextures;

public:
    Material();
    virtual ~Material() = default;

    void diffuse(const std::shared_ptr<Texture2D>&);
    void specular(const std::shared_ptr<Texture2D>&);
    void use(const ShaderProgram&) const;
};