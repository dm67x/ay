#include "material.h"
#include "texture2d.h"

Material::Material()
    : m_diffuseTextures{},
    m_specularTextures{}
{
}

void Material::Diffuse(const std::shared_ptr<Texture2D>& texture)
{
    m_diffuseTextures.push_back(texture);
}

void Material::Specular(const std::shared_ptr<Texture2D>& texture)
{
    m_specularTextures.push_back(texture);
}

void Material::Use(const ShaderProgram& program) const
{
    size_t i = 0;

    for (; i < m_diffuseTextures.size(); i++) {
        m_diffuseTextures[i]->Bind(static_cast<GLuint>(i));
        program.Uniform("diffuse_" + std::to_string(i + 1), [&](GLint loc) {
            glUniform1ui(loc, static_cast<GLuint>(i));
        });
    }

    for (size_t j = 0; j < m_specularTextures.size(); j++) {
        m_specularTextures[j]->Bind(static_cast<GLuint>(i+j));
        program.Uniform("specular_" + std::to_string(j + 1), [&](GLint loc) {
            glUniform1ui(loc, static_cast<GLuint>(i+j));
        });
    }
}