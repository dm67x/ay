#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>

class Texture2D;
class Shader;

class Material
{
    std::string m_name;
    glm::vec4 m_baseColorFactor;
    float m_metallicFactor;
    float m_roughnessFactor;
    glm::vec3 m_emissiveFactor;
    std::shared_ptr<Texture2D> m_baseColorTexture;
    std::shared_ptr<Texture2D> m_metallicRoughnessTexture;
    std::shared_ptr<Texture2D> m_normalTexture;

private:
    friend class glTFLoader;

public:
    Material(const std::string&);
    virtual ~Material() = default;

public:
    void use(const Shader&) const;

public:
    inline const std::string& name() const { return m_name; }
}; 