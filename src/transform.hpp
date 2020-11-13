#pragma once

#include "math.hpp"

struct Transform {
    Vec3 origin;
    Vec3 position;
    Vec3 scale;
    Vec3 rotation;

    ///
    /// @brief Constructor
    ///
    Transform()
        : origin(0.f, 0.f, 0.f),
        position(0.f, 0.f, 0.f),
        scale(1.f, 1.f, 1.f),
        rotation(0.f, 0.f, 0.f)
    {
    }

    ///
    /// @brief Get model matrix
    /// @return Matrix4 result
    ///
    inline Mat4 getTransform() const {
        Mat4 toOrigin = Mat4::translate(-origin);
        Mat4 translation = Mat4::translate(position);
        Mat4 scaling = Mat4::scale(scale);
        Mat4 rotate = Mat4::rotateX(radians(rotation.x)) * Mat4::rotateY(radians(rotation.y)) * Mat4::rotateZ(radians(rotation.z));
        return translation * rotate * scaling * toOrigin;
    }
};