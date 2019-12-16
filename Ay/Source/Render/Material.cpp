#include "Render/Material.hpp"
#include "Render/Texture2D.hpp"
#include "render/Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Material::Material(const std::string& name)
    : m_name{ name },
    m_diffuseTextures{},
    m_specularTextures{},
    m_Ka{ glm::vec3(1) },
    m_Kd{ glm::vec3(0) },
    m_Ks{ glm::vec3(0) },
    m_Ke{ glm::vec3(0) },
    m_Ns{ 0 },
    m_Ni{ 0 }
{
}

std::vector<Material> Material::load(const std::string& filename)
{
    std::vector<Material> materials = {};
    int i = -1;

    std::ifstream file(filename);
    if (file.is_open()) {
        for (std::string line; std::getline(file, line);) {
            if (line.substr(0, 7) == "newmtl ") {
                i++;
                std::stringstream ss(line.substr(7));
                materials.push_back(Material(ss.str()));
            }
            else if (line.substr(0, 3) == "Ka ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                materials[i].m_Ka = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Kd ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                materials[i].m_Kd = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Ks ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                materials[i].m_Ks = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Ke ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                materials[i].m_Ke = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Ni ") {
                std::stringstream ss(line.substr(3));
                float x;
                ss >> x;
                materials[i].m_Ni = x;
            }
            else if (line.substr(0, 3) == "Ns ") {
                std::stringstream ss(line.substr(3));
                float x;
                ss >> x;
                materials[i].m_Ns = x;
            }
            else if (line.substr(0, 2) == "d ") {
                /*std::stringstream ss(line.substr(2));
                float x;
                ss >> x;
                materials[i].m_d = x;*/
            }
            else if (line.substr(0, 6) == "illum ") {
                //std::stringstream ss(line.substr(6));
            }
        }
        
        file.close();
    }

    return materials;
}

void Material::diffuse(const Texture2D& texture)
{
    m_diffuseTextures.push_back(std::ref(texture));
}

void Material::specular(const Texture2D& texture)
{
    m_specularTextures.push_back(std::ref(texture));
}

void Material::use(const ShaderProgram& program) const
{
    // Textures
    size_t i = 0;
    for (; i < m_diffuseTextures.size(); i++) {
        m_diffuseTextures[i].get().bind(static_cast<GLuint>(i));
        program.uniform("diffuse_" + std::to_string(i + 1),
            static_cast<GLint>(i));
    }

    for (size_t j = 0; j < m_specularTextures.size(); j++) {
        m_specularTextures[j].get().bind(static_cast<GLuint>(i+j));
        program.uniform("specular_" + std::to_string(j + 1),
            static_cast<GLint>(i + j));
    }

    program.uniform("Ka", m_Ka);
    program.uniform("Ks", m_Ks);
    program.uniform("Kd", m_Kd);
    program.uniform("Ke", m_Ke);
    program.uniform("Ns", m_Ns);
    program.uniform("Ni", m_Ni);
}

void Material::draw(const ShaderProgram& program) const
{
    this->use(program);
}