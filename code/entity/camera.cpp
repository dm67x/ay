#include "camera.hpp"
#include "shader/shader.hpp"

Camera::Camera(const std::string& name, float aspectRatio)
    : Entity(name),
    m_target{ glm::vec3(0) },
    m_fov{ glm::radians(90.f) },
    m_zNear{ 0.1f },
    m_zFar{ 100.f },
    m_aspectRatio{ aspectRatio }
{
}

void Camera::target(const glm::vec3& target)
{
    m_target = target;
}

void Camera::aspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
}

glm::mat4 Camera::viewMatrix() const
{
    auto position = transform() * glm::vec4(1.f);

    return glm::lookAt(
        glm::vec3(position.x, position.y, position.z),
        m_target,
        glm::vec3(0, 1, 0));
}

glm::mat4 Camera::projectionMatrix() const
{
    return glm::perspective(
        m_fov,
        m_aspectRatio,
        m_zNear,
        m_zFar
    );
}

void Camera::draw(const Shader& program) const
{
    auto position = transform() * glm::vec4(1.f);

    program.uniform("projectionMatrix", projectionMatrix());
    program.uniform("viewMatrix", viewMatrix());
    program.uniform("cameraPosition", 
        glm::vec3(position.x, position.y, position.z));
}