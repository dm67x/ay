#include "Render/Camera.hpp"

Camera::Camera(const glm::vec3& position, const glm::quat& rotation)
    : m_position{ position },
    m_rotation{ rotation }
{
}

Camera::Camera()
    : Camera(glm::vec3(0), glm::identity<glm::quat>())
{
}

void Camera::rotate(const glm::quat& rotation)
{
    m_rotation = rotation;
}

void Camera::translate(const glm::vec3& position)
{
    m_position = position;
}

glm::mat4 Camera::view() const
{
    return glm::lookAt(
        m_position, 
        m_position + glm::vec3(0, 0, 1), 
        glm::vec3(0, 1, 0));
}