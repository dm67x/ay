#include "transform.hpp"

#include <glm/gtx/matrix_decompose.hpp>

Transform::Transform()
    : m_position{ glm::vec3(0) },
    m_rotation{ glm::identity<glm::quat>() },
    m_rotateAround{ glm::identity<glm::quat>() },
    m_scale{ glm::vec3(1) }
{
}

void Transform::position(const glm::vec3& position)
{
    m_position = position;
}

void Transform::position(float x, float y, float z)
{
    position(glm::vec3(x, y, z));
}

void Transform::rotate(const glm::quat& quaternion)
{
    m_rotation = quaternion;
}

void Transform::rotateX(float angle)
{
    m_rotation = glm::rotate(
        glm::identity<glm::quat>(),
        glm::radians(angle), 
        glm::vec3(1, 0, 0));
}

void Transform::rotateY(float angle)
{
    m_rotation = glm::rotate(
        glm::identity<glm::quat>(),
        glm::radians(angle),
        glm::vec3(0, 1, 0));
}

void Transform::rotateZ(float angle)
{
    m_rotation = glm::rotate(
        glm::identity<glm::quat>(),
        glm::radians(angle),
        glm::vec3(0, 0, 1));
}

void Transform::rotateAround(const glm::quat& quaternion)
{
    m_rotateAround = quaternion;
}

void Transform::rotateAroundX(float angle)
{
    m_rotateAround = glm::rotate(
        glm::identity<glm::quat>(),
        glm::radians(angle),
        glm::vec3(1, 0, 0));
}

void Transform::rotateAroundY(float angle)
{
    m_rotateAround = glm::rotate(
        glm::identity<glm::quat>(),
        glm::radians(angle),
        glm::vec3(0, 1, 0));
}

void Transform::rotateAroundZ(float angle)
{
    m_rotateAround = glm::rotate(
        glm::identity<glm::quat>(),
        glm::radians(angle),
        glm::vec3(0, 0, 1));
}

void Transform::scale(const glm::vec3& scale)
{
    m_scale = scale;
}

void Transform::scale(float x, float y, float z)
{
    scale(glm::vec3(x, y, z));
}

glm::mat4 Transform::transform() const
{
    auto translation = glm::translate(glm::mat4(1), m_position);
    auto ntranslation = glm::translate(glm::mat4(1), -m_position);
    auto rotate = glm::toMat4(m_rotation);
    auto rotateAround = glm::toMat4(m_rotateAround);
    auto scale = glm::scale(glm::mat4(1), m_scale);
    
    return scale * rotateAround * 
        translation * rotate * ntranslation * translation;
}