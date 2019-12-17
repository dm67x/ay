#include "Render/Material.hpp"
#include "Render/Texture2D.hpp"
#include "render/Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Material::Material(const std::string& name)
    : m_name{ name },
    m_parameters{}
{
}

bool Material::load(const std::string& filename)
{
    int i = -1;

    std::ifstream file(filename);
    if (file.is_open()) {
        for (std::string line; std::getline(file, line);) {
            if (line.substr(0, 7) == "newmtl ") {
                i++;
                std::stringstream ss(line.substr(7));
                MaterialParameters param;
                param.name = ss.str();
                m_parameters.push_back(param);
            }
            else if (line.substr(0, 3) == "Ka ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                m_parameters[i].Ka = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Kd ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                m_parameters[i].Kd = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Ks ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                m_parameters[i].Ks = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Ke ") {
                std::stringstream ss(line.substr(3));
                float x, y, z;
                ss >> x >> y >> z;
                m_parameters[i].Ke = glm::vec3(x, y, z);
            }
            else if (line.substr(0, 3) == "Ni ") {
                std::stringstream ss(line.substr(3));
                float x;
                ss >> x;
                m_parameters[i].Ni = x;
            }
            else if (line.substr(0, 3) == "Ns ") {
                std::stringstream ss(line.substr(3));
                float x;
                ss >> x;
                m_parameters[i].Ns = x;
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
        return true;
    }

    return false;
}

void Material::use(const ShaderProgram& program) const
{
    for (auto params : m_parameters) {
        program.uniform("Ka", params.Ka);
        program.uniform("Ks", params.Ks);
        program.uniform("Kd", params.Kd);
        program.uniform("Ke", params.Ke);
        program.uniform("Ns", params.Ns);
        program.uniform("Ni", params.Ni);
    }
}