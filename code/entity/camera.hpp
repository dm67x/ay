#pragma once

#include "entity.hpp"

#include <glm/glm.hpp>

class Camera : public Entity
{
    glm::vec3 m_target;
    glm::mat4 m_projection;

public:
    Camera();
    ~Camera() = default;

public:
    void target(const glm::vec3&);
    void projection(const glm::mat4&);
    void draw(const Shader&) const override;
    glm::mat4 view() const;

public:
    inline glm::vec3& target() { return m_target; }
    inline glm::mat4& projection() { return m_projection; }
};