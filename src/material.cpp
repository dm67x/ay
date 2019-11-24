#include "material.h"
#include "texture2d.h"

Material::Material()
    : m_diffuseTextures{},
    m_specularTextures{}
{
}

void Material::diffuse(const std::shared_ptr<Texture2D>& texture)
{
    m_diffuseTextures.push_back(texture);
}

void Material::specular(const std::shared_ptr<Texture2D>& texture)
{
    m_specularTextures.push_back(texture);
}

void Material::use(const ShaderProgram& program) const
{
    size_t i = 0;

    for (; i < m_diffuseTextures.size(); i++) {
        m_diffuseTextures[i]->bind(static_cast<GLuint>(i));
        program.uniform("diffuse_" + std::to_string(i + 1), [&](GLint loc) {
            glUniform1ui(loc, static_cast<GLuint>(i));
        });
    }

    for (size_t j = 0; j < m_specularTextures.size(); j++) {
        m_specularTextures[j]->bind(static_cast<GLuint>(i+j));
        program.uniform("specular_" + std::to_string(j + 1), [&](GLint loc) {
            glUniform1ui(loc, static_cast<GLuint>(i+j));
        });
    }
}