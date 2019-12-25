#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::quat m_rotateAround;
    glm::vec3 m_scale;

public:
    Transform();

public:
    void position(const glm::vec3&);
    void position(float, float, float);
    void rotate(const glm::quat&);
    void rotateX(float);
    void rotateY(float);
    void rotateZ(float);
    void rotateAround(const glm::quat&);
    void rotateAroundX(float);
    void rotateAroundY(float);
    void rotateAroundZ(float);
    void scale(const glm::vec3&);
    void scale(float, float, float);

    glm::mat4 transform() const;
};