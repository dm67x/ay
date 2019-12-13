#include "material.h"
#include "texture2d.h"

#include <iostream>

Material::Material(
    const Shader<GL_VERTEX_SHADER>& vert, 
    const Shader<GL_FRAGMENT_SHADER>& frag
)
    : m_diffuseTextures{},
    m_specularTextures{},
    m_program{ vert, frag }
{
    m_program.build();
}

void Material::diffuse(const Texture2D& texture)
{
    m_diffuseTextures.push_back(std::ref(texture));
}

void Material::specular(const Texture2D& texture)
{
    m_specularTextures.push_back(std::ref(texture));
}

void Material::use() const
{
    m_program.use();

    size_t i = 0;

    for (; i < m_diffuseTextures.size(); i++) {
        m_diffuseTextures[i].get().bind(static_cast<GLuint>(i));
        m_program.uniform("diffuse_" + std::to_string(i + 1), 
            static_cast<GLint>(i));
    }

    for (size_t j = 0; j < m_specularTextures.size(); j++) {
        m_specularTextures[j].get().bind(static_cast<GLuint>(i+j));
        m_program.uniform("specular_" + std::to_string(j + 1), 
            static_cast<GLint>(i + j));
    }
}

void Material::reset() const
{
    m_program.reset();
}