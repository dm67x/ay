#pragma once

#include "shader.h"
#include "ay.h"

#include <vector>
#include <memory>

class Texture2D;

class Material
{
    ShaderProgram m_program;
    std::vector<std::shared_ptr<Texture2D>> m_diffuseTextures;
    std::vector<std::shared_ptr<Texture2D>> m_specularTextures;

public:
    AY_API Material(const Shader<GL_VERTEX_SHADER>&, 
        const Shader<GL_FRAGMENT_SHADER>&);
    AY_API virtual ~Material() = default;

    AY_API void diffuse(const std::shared_ptr<Texture2D>&);
    AY_API void specular(const std::shared_ptr<Texture2D>&);
    AY_API void use() const;
    AY_API void reset() const;
    
    inline AY_API ShaderProgram& program() { return m_program; }
}; 