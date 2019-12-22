#include "camera.hpp"
#include "shader/shader.hpp"

Camera::Camera()
    : m_target{ glm::vec3(0, 0, 1) },
    m_projection{ glm::mat4() }
{
}

void Camera::target(const glm::vec3& target)
{
    m_target = target;
}

glm::mat4 Camera::view() const
{
    auto position = transform() * glm::vec4(1.f);

    return glm::lookAt(
        glm::vec3(position.x, position.y, position.z),
        m_target,
        glm::vec3(0, 1, 0));
}

void Camera::projection(const glm::mat4& p)
{
    m_projection = p;
}

void Camera::draw(const Shader& program) const
{
    auto position = transform() * glm::vec4(1.f);

    program.uniform("projectionMatrix", m_projection);
    program.uniform("viewMatrix", view());
    program.uniform("cameraPosition", 
        glm::vec3(position.x, position.y, position.z));
}