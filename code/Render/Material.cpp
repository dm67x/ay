#include "Material.hpp"
#include "Texture2D.hpp"
#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Material::Material(const std::string& name)
    : m_name{ name }
{
}

void Material::use(const ShaderProgram& program) const
{
    program.uniform("Ka", m_Ka);
    program.uniform("Ks", m_Ks);
    program.uniform("Kd", m_Kd);
    program.uniform("Ke", m_Ke);
    program.uniform("Ns", m_Ns);
    program.uniform("Ni", m_Ni);
}