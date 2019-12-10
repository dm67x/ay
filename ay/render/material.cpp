#include "material.h"
#include "texture2d.h"

#include <iostream>

Material::Material(const std::string& vert, const std::string& frag)
    : m_diffuseTextures{},
    m_specularTextures{},
    m_vertexShader{ GL_VERTEX_SHADER },
    m_fragmentShader{ GL_FRAGMENT_SHADER },
    m_program{ m_vertexShader, m_fragmentShader }
{
    m_vertexShader.fromFile(vert);
    m_fragmentShader.fromFile(frag);
    m_program.build();
}

void Material::diffuse(const std::shared_ptr<Texture2D>& texture)
{
    m_diffuseTextures.push_back(texture);
}

void Material::specular(const std::shared_ptr<Texture2D>& texture)
{
    m_specularTextures.push_back(texture);
}

void Material::use() const
{
    m_program.use();

    size_t i = 0;

    for (; i < m_diffuseTextures.size(); i++) {
        m_diffuseTextures[i]->bind(static_cast<GLuint>(i));
        m_program.uniform("diffuse_" + std::to_string(i + 1), 
            static_cast<GLint>(i));
    }

    for (size_t j = 0; j < m_specularTextures.size(); j++) {
        m_specularTextures[j]->bind(static_cast<GLuint>(i+j));
        m_program.uniform("specular_" + std::to_string(j + 1), 
            static_cast<GLint>(i + j));
    }
}

void Material::reset() const
{
    m_program.reset();
}