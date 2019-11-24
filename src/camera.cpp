#include "camera.h"

#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(const glm::vec3& position)
    : m_direction{ glm::vec3(1, 0, 0) },
    m_position{ position },
    m_view{},
    m_speed{ 0.2f }
{
}

Camera::Camera()
    : Camera(glm::vec3(0))
{
}

void Camera::rotate(float amount, const glm::vec3& axis)
{
    m_direction = glm::rotate(m_direction, amount * m_speed, axis);
}

void Camera::move(const glm::vec3& direction)
{
    m_position += direction;
}

void Camera::move(CameraMovementType type)
{
    switch (type) {
    case FORWARD:
        m_position += m_direction;
        break;

    case BACKWARD:
        m_position -= m_direction;
        break;

    case STRAFE_LEFT:
        m_position += glm::cross(m_direction, glm::vec3(0, 1, 0));
        break;

    case STRAFE_RIGHT:
        m_position -= glm::cross(m_direction, glm::vec3(0, 1, 0));
        break;
    }
}

void Camera::update(float time)
{
    float radius = 10.0f;
    float camX = sin(time) * radius;
    float camZ = cos(time) * radius;
    //float camY = cos(time) * sin(time) * radius;
    m_view = glm::lookAt(glm::vec3(camX, 0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}