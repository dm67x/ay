#include "Render/Material.hpp"
#include "Render/Texture2D.hpp"
#include "render/Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Material::Material(const std::string& name)
    : m_name{ name },
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

void Material::use(const ShaderProgram& program) const
{
    program.uniform("Ka", m_Ka);
    program.uniform("Ks", m_Ks);
    program.uniform("Kd", m_Kd);
    program.uniform("Ke", m_Ke);
    program.uniform("Ns", m_Ns);
    program.uniform("Ni", m_Ni);
}