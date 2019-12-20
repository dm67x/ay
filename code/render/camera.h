#pragma once

#include "scene/entity.h"
#include "transform.h"

#include <glm/glm.hpp>

class Camera : public SceneEntity
{
    glm::vec3 m_target;

public:
    Camera();
    ~Camera() = default;

public:
    void target(const glm::vec3&);
    void draw(const Shader&) const override;
    glm::mat4 view() const;

public:
    inline glm::vec3& target() { return m_target; }
};