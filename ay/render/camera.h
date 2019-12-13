#pragma once

#include "scene/entity.h"
#include "ay.h"

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
    inline AY_API glm::vec3& position() { return m_position; }
    inline AY_API glm::quat& rotation() { return m_rotation; }
};