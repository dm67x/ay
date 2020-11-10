#pragma once

#include <cmath>

struct Vec3 {
    float x;
    float y;
    float z;

    ///
    /// @brief New vector with coordinates (0, 0, 0)
    ///
    Vec3() : Vec3(0.f, 0.f, 0.f) {}

    ///
    /// @brief New vector with coordinates (x, y, z)
    /// @param x
    /// @param y
    /// @param z
    ///
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    ///
    /// @brief Add two vectors
    /// @param v1 First vector
    /// @param v2 Second vector
    /// @return The result of the addition
    ///
    friend Vec3 operator+(const Vec3& v1, const Vec3& v2) {
        return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    ///
    /// @brief Subtract two vectors
    /// @param v1 First vector
    /// @param v2 Second vector
    /// @return The result of the subtraction
    ///
    friend Vec3 operator-(const Vec3& v1, const Vec3& v2) {
        return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    ///
    /// @brief Multiply vector coordinates by a float number
    /// @param v1 The vector
    /// @param v2 The number
    /// @return The result of the multiplication
    ///
    friend Vec3 operator*(const Vec3& v1, float v2) {
        return Vec3(v1.x * v2, v1.y * v2, v1.z * v2);
    }

    ///
    /// @brief Cross product of two vectors
    /// @param v1 First vector
    /// @param v2 Second vector
    /// @return The result of the cross product
    ///
    friend Vec3 operator*(const Vec3& v1, const Vec3& v2) {
        return Vec3(
            v1.y * v2.z - v1.z * v2.y, 
            v1.z * v2.x - v1.x * v2.z, 
            v1.x * v2.y - v1.y * v2.x);
    }
    
    ///
    /// @brief Dot product of two vectors
    /// @param v Second vector
    /// @return The result of the dot product
    ///
    inline float dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    ///
    /// @brief Get the length of the vector
    /// @return The length of the vector
    ///
    inline float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    ///
    /// @brief Normalize the vector
    /// @return The new normalized vector
    ///
    inline Vec3 normalize() const {
        float len = length();
        return len > 0.f ? Vec3(x / len, y / len, z / len) : Vec3(x, y, z);
    }
};

struct Mat4 {
    float data[16];
};