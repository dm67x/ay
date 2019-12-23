#include "material.hpp"
#include "shader.hpp"
#include "texture2d.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Material::Material(const std::string& name)
    : m_name{ name },
    m_baseColorFactor{ glm::vec4(1) },
    m_metallicFactor{ 0 },
    m_roughnessFactor{ 0 },
    m_emissiveFactor{ glm::vec3(0) },
    m_baseColorTexture{ nullptr },
    m_metallicRoughnessTexture{ nullptr },
    m_normalTexture{ nullptr }
{
}

void Material::use(const Shader& program) const
{
    program.uniform("baseColorFactor", m_baseColorFactor);
    program.uniform("metallicFactor", m_metallicFactor);
    program.uniform("roughnessFactor", m_roughnessFactor);
    program.uniform("emissiveFactor", m_emissiveFactor);
    program.uniform("isBaseColorTexture", 0);
    program.uniform("isMetallicRoughnessTexture", 0);
    program.uniform("isNormalTexture", 0);

    if (m_baseColorTexture) {
        program.uniform("isBaseColorTexture", 1);
        m_baseColorTexture->bind(0);
        program.uniform("baseColorTexture", 0);
    }

    if (m_metallicRoughnessTexture) {
        program.uniform("isMetallicRoughnessTexture", 1);
        m_metallicRoughnessTexture->bind(1);
        program.uniform("metallicRoughnessTexture", 1);
    }

    if (m_metallicRoughnessTexture) {
        program.uniform("isNormalTexture", 1);
        m_normalTexture->bind(2);
        program.uniform("normalTexture", 2);
    }
}