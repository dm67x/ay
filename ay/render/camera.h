#pragma once

#include "scene/entity.h"
#include "ay.h"

#include <glm/glm.hpp>

enum CameraMovementType
{
    FORWARD,
    BACKWARD,
    STRAFE_LEFT,
    STRAFE_RIGHT
};

class Camera : public SceneEntity
{
    glm::vec3 m_direction;
    glm::vec3 m_position;
    glm::mat4 m_view;
    float m_speed;

public:
    AY_API Camera();
    AY_API Camera(const glm::vec3&);
    AY_API ~Camera() = default;

    AY_API void rotate(float, const glm::vec3&);
    AY_API void move(const glm::vec3&);
    AY_API void move(CameraMovementType);
    AY_API void update(double);
    AY_API void draw() const override {}

    inline AY_API glm::mat4 view() const { return m_view; }
    inline AY_API glm::vec3 position() const { return m_position; }
};