#pragma once

#include "Scene/Entity.hpp"
#include "Ay.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera : public SceneEntity
{
    glm::vec3 m_position;
    glm::quat m_rotation;

public:
    AY_API Camera();
    AY_API Camera(const glm::vec3&, const glm::quat&);
    AY_API ~Camera() = default;

    AY_API void rotate(const glm::quat&);
    AY_API void translate(const glm::vec3&);
    AY_API void draw() const override {}

    AY_API glm::mat4 view() const;
    inline glm::vec3& position() { return m_position; }
    inline glm::quat& rotation() { return m_rotation; }
};