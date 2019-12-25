#pragma once

#include "rendering/transform.hpp"

#include <string>

class Shader;

class Entity : public Transform
{
    std::string m_name;

protected:
    Entity(const std::string& name) : m_name{ name } {}

public:
    void name(const std::string& name) { m_name = name; }
    virtual void draw(const Shader&, const Transform& = Transform()) const = 0;

public:
    inline const std::string& name() const { return m_name; }
};