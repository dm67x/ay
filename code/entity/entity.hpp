#pragma once

#include "rendering/transform.hpp"

#include <string>
#include <memory>

class Shader;

class Entity : public Transform
{
    std::string m_name;
    std::shared_ptr<Entity> m_parent;

protected:
    Entity(const std::string& name) : m_name{ name }, m_parent{ nullptr } {}

public:
    void parent(const std::shared_ptr<Entity>& parent) { m_parent = parent; }
    void name(const std::string& name) { m_name = name; }
    virtual void draw(const Shader&, const glm::mat4& = glm::mat4(1)) const = 0;

public:
    inline const std::string& name() const { return m_name; }
    inline const std::shared_ptr<Entity>& parent() const { return m_parent; }
};