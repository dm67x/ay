#pragma once

#include "Ay.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <string>

class Texture2D;
class ShaderProgram;

class Material
{
    std::string m_name;
    glm::vec3 m_Ka;
    glm::vec3 m_Kd;
    glm::vec3 m_Ks;
    glm::vec3 m_Ke;
    float m_Ns;
    float m_Ni;

public:
    AY_API Material(const std::string&);
    AY_API virtual ~Material() = default;

    AY_API static std::vector<Material> load(const std::string&);
    AY_API void use(const ShaderProgram&) const;
}; 