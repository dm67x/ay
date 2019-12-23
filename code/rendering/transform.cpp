#include "transform.hpp"

Transform::Transform()
    : m_position{ glm::vec3(0) },
    m_rAxis{ glm::vec3(0, 1, 0) },
    m_rAmount{ 0 },
    m_size{ glm::vec3(1) }
{
}

void Transform::translate(const glm::vec3& position)
{
    m_position = position;
}

void Transform::rotate(float angle, const glm::vec3& axis)
{
    m_rAmount = angle;
    m_rAxis = axis;
}

void Transform::scale(const glm::vec3& scale)
{
    m_size = scale;
}

glm::mat4 Transform::transform() const
{
    auto translate = glm::translate(glm::mat4(1.f), m_position);
    auto rotate = glm::rotate(glm::mat4(1.f), m_rAmount, m_rAxis);
    auto scaling = glm::scale(glm::mat4(1.f), m_size);
    return scaling * rotate * translate;
}