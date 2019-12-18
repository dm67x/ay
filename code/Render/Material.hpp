#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>

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
    int m_isMapKd;
    std::shared_ptr<Texture2D> m_mapKd;

private:
    friend class Model;

public:
    Material(const std::string&);
    virtual ~Material() = default;

public:
    void use(const ShaderProgram&) const;

public:
    inline const std::string& name() const { return m_name; }
}; 