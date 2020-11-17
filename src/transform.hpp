#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
    glm::vec3 origin;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    ///
    /// @brief Constructor
    ///
    Transform()
        : origin(0.f, 0.f, 0.f),
        position(0.f, 0.f, 0.f),
        scale(1.f, 1.f, 1.f),
        rotation()
    {
    }

    ///
    /// @brief Get model matrix
    /// @return Matrix4 result
    ///
    inline glm::mat4 getTransform() const {
        glm::mat4 toOrigin = glm::translate(glm::mat4(1.f), -origin);
        glm::mat4 translation = glm::translate(glm::mat4(1.f), position);
        glm::mat4 scaling = glm::scale(glm::mat4(1.f), scale);
        glm::mat4 rotate = glm::toMat4(glm::quat(rotation));
        return translation * rotate * scaling * toOrigin;
    }
};