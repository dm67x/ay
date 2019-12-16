#pragma once

#include "Scene/Entity.hpp"
#include "Ay.hpp"
#include "Transform.hpp"

#include <glm/glm.hpp>

class Camera : public SceneEntity
{
    glm::vec3 m_target;

public:
    AY_API Camera();
    AY_API ~Camera() = default;

    AY_API void target(const glm::vec3&);
    AY_API void draw(const ShaderProgram&) const override;
    AY_API glm::mat4 view() const;

    inline glm::vec3& target() { return m_target; }
};