#include "transform.hpp"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

Transform::Transform()
    : m_transform{ glm::mat4(1) }
{
}

void Transform::translate(const glm::vec3& position)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(
        m_transform, 
        scale, 
        rotation, 
        translation, 
        skew, 
        perspective);

    // Find rotation
    glm::mat4 rotate = glm::mat4(1);
    auto eulerAngles = glm::eulerAngles(rotation);
    auto eulerAnglesLength = glm::length(eulerAngles);

    rotate = glm::translate(rotate, position);
    if (eulerAnglesLength != 0) {
        eulerAngles /= eulerAnglesLength;
        rotate = glm::rotate(rotate, eulerAnglesLength, eulerAngles);
    }
    rotate = glm::translate(rotate, -position);

    // Basic transformation
    glm::mat4 translate, scaling;
    translate = glm::translate(glm::mat4(1), position);
    scaling = glm::scale(glm::mat4(1), scale);

    m_transform = scaling * rotate * translate;
}

void Transform::rotate(float angle, const glm::vec3& axis)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(
        m_transform,
        scale,
        rotation,
        translation,
        skew,
        perspective);

    // Find rotation
    glm::mat4 rotate = glm::mat4(1);

    rotate = glm::translate(rotate, translation);
    rotate = glm::rotate(rotate, glm::radians(angle), axis);
    rotate = glm::translate(rotate, -translation);

    // Basic transformation
    glm::mat4 translate, scaling;
    translate = glm::translate(glm::mat4(1), translation);
    scaling = glm::scale(glm::mat4(1), scale);

    m_transform = scaling * rotate * translate;
}

void Transform::scale(const glm::vec3& scale)
{
    glm::vec3 _scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(
        m_transform,
        _scale,
        rotation,
        translation,
        skew,
        perspective);

    // Find rotation
    glm::mat4 rotate = glm::mat4(1);
    auto eulerAngles = glm::eulerAngles(rotation);
    auto eulerAnglesLength = glm::length(eulerAngles);

    rotate = glm::translate(rotate, translation);
    if (eulerAnglesLength != 0) {
        eulerAngles /= eulerAnglesLength;
        rotate = glm::rotate(rotate, eulerAnglesLength, eulerAngles);
    }
    rotate = glm::translate(rotate, -translation);

    // Basic transformation
    glm::mat4 translate, scaling;
    translate = glm::translate(glm::mat4(1), translation);
    scaling = glm::scale(glm::mat4(1), scale);

    m_transform = scaling * rotate * translate;
}

glm::mat4 Transform::transform() const
{
    return m_transform;
}