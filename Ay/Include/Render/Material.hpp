#pragma once

#include "Shader.hpp"
#include "Ay.hpp"

#include <vector>
#include <memory>

class Texture2D;

class Material
{
    ShaderProgram m_program;
    std::vector<std::reference_wrapper<const Texture2D>> m_diffuseTextures;
    std::vector<std::reference_wrapper<const Texture2D>> m_specularTextures;

public:
    AY_API Material(const Shader<GL_VERTEX_SHADER>&,
        const Shader<GL_FRAGMENT_SHADER>&);
    AY_API virtual ~Material() = default;

    AY_API void diffuse(const Texture2D&);
    AY_API void specular(const Texture2D&);
    AY_API void use() const;
    AY_API void reset() const;
    
    inline ShaderProgram& program() { return m_program; }
}; 