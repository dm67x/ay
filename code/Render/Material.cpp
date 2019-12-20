#include "Material.hpp"
#include "Texture2D.hpp"
#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Material::Material(const std::string& name)
    : m_name{ name },
    m_Ka{ glm::vec3(1) },
    m_Kd{ glm::vec3(1) },
    m_Ks{ glm::vec3(0) },
    m_Ke{ glm::vec3(0) },
    m_Ns{ 0 },
    m_Ni{ 0 },
    m_isMapKd{ 0 },
    m_mapKd{ nullptr }
{
}

void Material::use(const Shader& program) const
{
    program.uniform("Ka", m_Ka);
    program.uniform("Ks", m_Ks);
    program.uniform("Kd", m_Kd);
    program.uniform("Ke", m_Ke);
    program.uniform("Ns", m_Ns);
    program.uniform("Ni", m_Ni);
    program.uniform("mapKdValid", m_isMapKd);

    if (m_isMapKd && m_mapKd) {
        m_mapKd->bind(0);
        program.uniform("mapKd", 0);
    }
}