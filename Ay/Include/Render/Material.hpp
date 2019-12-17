#pragma once

#include "Ay.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <string>

class Texture2D;
class ShaderProgram;

struct MaterialParameters
{
    std::string name;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    glm::vec3 Ke;
    float Ns;
    float Ni;
};

class Material
{
    std::string m_name;
    std::vector<MaterialParameters> m_parameters;

public:
    AY_API Material(const std::string&);
    AY_API virtual ~Material() = default;

    AY_API bool load(const std::string&);
    AY_API void use(const ShaderProgram&) const;

    inline const std::string& name() const { return m_name; }
}; 