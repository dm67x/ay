#pragma once

#include "entity.hpp"

#include <glm/glm.hpp>

struct Light : public Entity
{
    glm::vec3 position = glm::vec3(0);
    glm::vec3 color = glm::vec3(1);
    float power = 10.f;

private:
    friend class glTFLoader;

public:
    Light(const std::string&);
    virtual void draw(const Shader&) const override;
};