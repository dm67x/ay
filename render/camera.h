#pragma once

#include "scene/entity.h"

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
    Camera();
    Camera(const glm::vec3&);
    ~Camera() = default;

    void rotate(float, const glm::vec3&);
    void move(const glm::vec3&);
    void move(CameraMovementType);
    void update(double);
    void draw() const override {}

    inline glm::mat4 view() const { return m_view; }
    inline glm::vec3 position() const { return m_position; }
};