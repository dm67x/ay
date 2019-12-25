#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
    glm::mat4 m_transform;

public:
    Transform();

public:
    void translate(const glm::vec3&);
    void rotate(float, const glm::vec3&);
    void scale(const glm::vec3&);
    glm::mat4 transform() const;
};