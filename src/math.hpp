#pragma once

#include <cmath>
#include <cstring>
#include <limits>
#include <cassert>
#include <ostream>

#define PI 3.14159265358979323846f

///
/// @brief Radians to Degrees
/// @param radians Radians
/// @return Degrees
///
inline float degrees(float radians) {
    return radians * (180.f / PI);
}

///
/// @brief Degrees to radians
/// @param degrees Degrees
/// @return Radians
///
inline float radians(float degrees) {
    return degrees * (PI / 180.f);
}

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
    /// @brief Equal operator
    /// @param v Vec3
    /// @return True if equal false otherwise
    ///
    inline bool operator==(const Vec3& v) {
        return x == v.x && y == v.y && z == v.z;
    }

    ///
    /// @brief Add the current vector with an other vector
    /// @param v1 Vector
    /// @return The reference to the current vector
    ///
    inline Vec3& operator+=(const Vec3& v1) {
        x += v1.x;
        y += v1.y;
        z += v1.z;
        return *this;
    }

    ///
    /// @brief Add the current vector with an other vector
    /// @param v1 Vector
    /// @return The reference to the current vector
    ///
    inline Vec3& operator-=(const Vec3& v1) {
        x -= v1.x;
        y -= v1.y;
        z -= v1.z;
        return *this;
    }

    ///
    /// @brief Format for output values in console
    /// @param out Out stream
    /// @param v Vec3
    /// @return Out stream
    ///
    friend std::ostream& operator<<(std::ostream& out, const Vec3& v) {
        out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return out;
    }

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
    /// @brief Negative vector
    /// @param v1 vector
    /// @return Negative vector
    ///
    friend Vec3 operator-(const Vec3& v1) {
        return Vec3(-v1.x, -v1.y, -v1.z);
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

struct Vec4 {
    float x;
    float y;
    float z;
    float w;

    ///
    /// @brief Constructor
    ///
    Vec4() : Vec4(0.f, 0.f, 0.f, 0.f) {}

    ///
    /// @brief Constructor
    /// @param x x
    /// @param y y
    /// @param z z
    /// @param w w 
    ///
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    ///
    /// @brief Convert to a Vec3
    /// @return Converted Vec3
    ///
    inline Vec3 toVec3() const {
        return Vec3(x / w, y / w, z / w);
    }
};

struct DeterminantException : public std::exception {
    const char* what() const noexcept override {
        return "Determinant is null";
    }
};

struct Mat4 {
    float r1[4];
    float r2[4];
    float r3[4];
    float r4[4];

    ///
    /// @brief Constructor
    ///
    Mat4() {
        std::memset(r1, 0, sizeof(r1));
        std::memset(r2, 0, sizeof(r2));
        std::memset(r3, 0, sizeof(r3));
        std::memset(r4, 0, sizeof(r4));
    }

    ///
    /// @brief Identity matrix
    /// @return Identity matrix
    ///
    static Mat4 identity() {
        Mat4 matrix;
        matrix.r1[0] = 1.f;
        matrix.r2[1] = 1.f;
        matrix.r3[2] = 1.f;
        matrix.r4[3] = 1.f;
        return matrix;
    }

    ///
    /// @brief Addition of two matrices
    /// @param m Matrix4
    /// @param n Matrix4
    /// @return Matrix4 result
    ///
    friend Mat4 operator+(const Mat4& m, const Mat4& n) {
        Mat4 matrix;
        for (int i = 0; i < 4; i++) matrix.r1[i] = m.r1[i] + n.r1[i];
        for (int i = 0; i < 4; i++) matrix.r2[i] = m.r2[i] + n.r2[i];
        for (int i = 0; i < 4; i++) matrix.r3[i] = m.r3[i] + n.r3[i];
        for (int i = 0; i < 4; i++) matrix.r4[i] = m.r4[i] + n.r4[i];
        return matrix;
    }

    ///
    /// @brief Addition of a matrix and a number
    /// @param m Matrix4
    /// @param n Number
    /// @return Matrix4 result
    ///
    friend Mat4 operator+(const Mat4& m, float n) {
        Mat4 matrix;
        for (int i = 0; i < 4; i++) matrix.r1[i] = m.r1[i] + n;
        for (int i = 0; i < 4; i++) matrix.r2[i] = m.r2[i] + n;
        for (int i = 0; i < 4; i++) matrix.r3[i] = m.r3[i] + n;
        for (int i = 0; i < 4; i++) matrix.r4[i] = m.r4[i] + n;
        return matrix;
    }

    ///
    /// @brief Multiply a matrix with a number
    /// @param m Matrix4
    /// @param n Number
    /// @return Matrix4 result
    ///
    friend Mat4 operator*(const Mat4& m, float n) {
        Mat4 matrix;
        for (int i = 0; i < 4; i++) matrix.r1[i] = m.r1[i] * n;
        for (int i = 0; i < 4; i++) matrix.r2[i] = m.r2[i] * n;
        for (int i = 0; i < 4; i++) matrix.r3[i] = m.r3[i] * n;
        for (int i = 0; i < 4; i++) matrix.r4[i] = m.r4[i] * n;
        return matrix;
    }

    ///
    /// @brief Multiply two matrices
    /// @param m Matrix4
    /// @param n Matrix4
    /// @return Matrix4 result
    ///
    friend Mat4 operator*(const Mat4& m, const Mat4& n) {
        Mat4 matrix;
        matrix.r1[0] = m.r1[0] * n.r1[0] + m.r1[1] * n.r2[0] + m.r1[2] * n.r3[0] + m.r1[3] * n.r4[0];
        matrix.r1[1] = m.r1[0] * n.r1[1] + m.r1[1] * n.r2[1] + m.r1[2] * n.r3[1] + m.r1[3] * n.r4[1];
        matrix.r1[2] = m.r1[0] * n.r1[2] + m.r1[1] * n.r2[2] + m.r1[2] * n.r3[2] + m.r1[3] * n.r4[2];
        matrix.r1[3] = m.r1[0] * n.r1[3] + m.r1[1] * n.r2[3] + m.r1[2] * n.r3[3] + m.r1[3] * n.r4[3];

        matrix.r2[0] = m.r2[0] * n.r1[0] + m.r2[1] * n.r2[0] + m.r2[2] * n.r3[0] + m.r2[3] * n.r4[0];
        matrix.r2[1] = m.r2[0] * n.r1[1] + m.r2[1] * n.r2[1] + m.r2[2] * n.r3[1] + m.r2[3] * n.r4[1];
        matrix.r2[2] = m.r2[0] * n.r1[2] + m.r2[1] * n.r2[2] + m.r2[2] * n.r3[2] + m.r2[3] * n.r4[2];
        matrix.r2[3] = m.r2[0] * n.r1[3] + m.r2[1] * n.r2[3] + m.r2[2] * n.r3[3] + m.r2[3] * n.r4[3];

        matrix.r3[0] = m.r3[0] * n.r1[0] + m.r3[1] * n.r2[0] + m.r3[2] * n.r3[0] + m.r3[3] * n.r4[0];
        matrix.r3[1] = m.r3[0] * n.r1[1] + m.r3[1] * n.r2[1] + m.r3[2] * n.r3[1] + m.r3[3] * n.r4[1];
        matrix.r3[2] = m.r3[0] * n.r1[2] + m.r3[1] * n.r2[2] + m.r3[2] * n.r3[2] + m.r3[3] * n.r4[2];
        matrix.r3[3] = m.r3[0] * n.r1[3] + m.r3[1] * n.r2[3] + m.r3[2] * n.r3[3] + m.r3[3] * n.r4[3];

        matrix.r4[0] = m.r4[0] * n.r1[0] + m.r4[1] * n.r2[0] + m.r4[2] * n.r3[0] + m.r4[3] * n.r4[0];
        matrix.r4[1] = m.r4[0] * n.r1[1] + m.r4[1] * n.r2[1] + m.r4[2] * n.r3[1] + m.r4[3] * n.r4[1];
        matrix.r4[2] = m.r4[0] * n.r1[2] + m.r4[1] * n.r2[2] + m.r4[2] * n.r3[2] + m.r4[3] * n.r4[2];
        matrix.r4[3] = m.r4[0] * n.r1[3] + m.r4[1] * n.r2[3] + m.r4[2] * n.r3[3] + m.r4[3] * n.r4[3];
        return matrix;
    }

    ///
    /// @brief Multiply the matrix with a vector
    /// @param m Matrix4
    /// @param n Vector4
    /// @return Vector4 result
    ///
    friend Vec4 operator*(const Mat4& m, const Vec4& n) {
        Vec4 result;
        result.x = m.r1[0] * n.x + m.r1[1] * n.y + m.r1[2] * n.z + m.r1[3] * n.w;
        result.y = m.r2[0] * n.x + m.r2[1] * n.y + m.r2[2] * n.z + m.r2[3] * n.w;
        result.z = m.r3[0] * n.x + m.r3[1] * n.y + m.r3[2] * n.z + m.r3[3] * n.w;
        result.w = m.r4[0] * n.x + m.r4[1] * n.y + m.r4[2] * n.z + m.r4[3] * n.w;
        return result;
    }

    ///
    /// @brief Transpose matrix
    /// @return Matrix4 result
    ///
    Mat4 transpose() const {
        Mat4 matrix;
        matrix.r1[0] = r1[0];
        matrix.r2[0] = r1[1];
        matrix.r3[0] = r1[2];
        matrix.r4[0] = r1[3];

        matrix.r1[1] = r2[0];
        matrix.r2[1] = r2[1];
        matrix.r3[1] = r2[2];
        matrix.r4[1] = r2[3];

        matrix.r1[2] = r3[0];
        matrix.r2[2] = r3[1];
        matrix.r3[2] = r3[2];
        matrix.r4[2] = r3[3];

        matrix.r1[3] = r4[0];
        matrix.r2[3] = r4[1];
        matrix.r3[3] = r4[2];
        matrix.r4[3] = r4[3];
        return matrix;
    }

    ///
    /// @brief Calculate the determinant
    /// @return Determinant
    ///
    float determinant() const {
        float a11 = r1[0];
        float a12 = r1[1];
        float a13 = r1[2];
        float a14 = r1[3];

        float a21 = r2[0];
        float a22 = r2[1];
        float a23 = r2[2];
        float a24 = r2[3];

        float a31 = r3[0];
        float a32 = r3[1];
        float a33 = r3[2];
        float a34 = r3[3];

        float a41 = r4[0];
        float a42 = r4[1];
        float a43 = r4[2];
        float a44 = r4[3];

        return a11 * a22 * a33 * a44 + a11 * a23 * a34 * a42 + a11 * a24 * a32 * a43
            - a11 * a24 * a33 * a42 - a11 * a23 * a32 * a44 - a11 * a22 * a34 * a43
            - a12 * a21 * a33 * a44 - a13 * a21 * a34 * a42 - a14 * a21 * a32 * a43
            + a14 * a21 * a33 * a42 + a13 * a21 * a32 * a44 + a12 * a21 * a34 * a43
            + a12 * a23 * a31 * a44 + a13 * a24 * a31 * a42 + a14 * a22 * a31 * a43
            - a14 * a23 * a31 * a42 - a13 * a22 * a31 * a44 - a12 * a24 * a31 * a43
            - a12 * a23 * a34 * a41 - a13 * a24 * a32 * a41 - a14 * a22 * a33 * a41
            + a14 * a23 * a32 * a41 + a13 * a22 * a34 * a41 + a12 * a24 * a33 * a41; 
    }

    ///
    /// @brief Adjugate matrix
    /// @return Matrix4 result
    ///
    Mat4 adjugate() const {
        float a11 = r1[0];
        float a12 = r1[1];
        float a13 = r1[2];
        float a14 = r1[3];

        float a21 = r2[0];
        float a22 = r2[1];
        float a23 = r2[2];
        float a24 = r2[3];

        float a31 = r3[0];
        float a32 = r3[1];
        float a33 = r3[2];
        float a34 = r3[3];

        float a41 = r4[0];
        float a42 = r4[1];
        float a43 = r4[2];
        float a44 = r4[3];

        Mat4 matrix;
        matrix.r1[0] = a22 * a33 * a44 + a23 * a34 * a42 + a24 * a32 * a43 - a24 * a33 * a42 - a23 * a32 * a44 - a22 * a34 * a43;
        matrix.r1[1] = -a12 * a33 * a44 - a13 * a34 * a42 - a14 * a32 * a43 + a14 * a33 * a42 + a13 * a32 * a44 + a12 * a34 * a43;
        matrix.r1[2] = a12 * a23 * a44 + a13 * a24 * a42 + a14 * a22 * a43 - a14 * a23 * a42 - a13 * a22 * a44 - a12 * a24 * a43;
        matrix.r1[3] = -a12 * a23 * a34 - a13 * a24 * a32 - a14 * a22 * a33 + a14 * a23 * a32 + a13 * a22 * a34 + a12 * a24 * a33;

        matrix.r2[0] = -a21 * a33 * a44 - a23 * a34 * a41 - a24 * a31 * a43 + a24 * a33 * a41 + a23 * a31 * a44 + a21 * a34 * a43;
        matrix.r2[1] = a11 * a33 * a44 + a13 * a34 * a41 + a14 * a31 * a43 - a14 * a33 * a41 - a13 * a31 * a44 - a11 * a34 * a43;
        matrix.r2[2] = -a11 * a23 * a44 - a13 * a24 * a41 - a14 * a21 * a43 + a14 * a23 * a41 + a13 * a21 * a44 + a11 * a24 * a43;
        matrix.r2[3] = a11 * a23 * a34 + a13 * a24 * a31 + a14 * a21 * a33 - a14 * a23 * a31 - a13 * a21 * a34 - a11 * a24 * a33;

        matrix.r3[0] = a21 * a32 * a44 + a22 * a34 * a41 + a24 * a31 * a42 - a24 * a32 * a41 - a22 * a31 * a44 - a21 * a34 * a42;
        matrix.r3[1] = -a11 * a32 * a44 - a12 * a34 * a41 - a14 * a31 * a42 + a14 * a32 * a41 + a12 * a31 * a44 + a11 * a34 * a42;
        matrix.r3[2] = a11 * a22 * a44 + a12 * a24 * a41 + a14 * a21 * a42 - a14 * a22 * a41 - a12 * a21 * a44 - a11 * a24 * a42;
        matrix.r3[3] = -a11 * a22 * a34 - a12 * a24 * a31 - a14 * a21 * a32 + a14 * a22 * a31 + a12 * a21 * a34 + a11 * a24 * a32;

        matrix.r4[0] = -a21 * a32 * a43 - a22 * a33 * a41 - a23 * a31 * a42 + a23 * a32 * a41 + a22 * a31 * a43 + a21 * a33 * a42;
        matrix.r4[1] = a11 * a32 * a43 + a12 * a33 * a41 + a13 * a31 * a42 - a13 * a32 * a41 - a12 * a31 * a43 - a11 * a33 * a42;
        matrix.r4[2] = -a11 * a22 * a43 - a12 * a23 * a41 - a13 * a21 * a42 + a13 * a22 * a41 + a12 * a21 * a43 + a11 * a23 * a42;
        matrix.r4[3] = a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31 - a12 * a21 * a33 - a11 * a23 * a32;

        return matrix;
    }

    ///
    /// @brief Inverse matrix
    /// @return Matrix4 result
    ///
    inline Mat4 inverse() const {
        float determinant = this->determinant();
        if (determinant > 0.f || determinant < 0.f) {
            return this->adjugate() * (1.f / this->determinant());
        } else {
            throw DeterminantException();
        }
    }

    ///
    /// @brief Translation matrix
    /// @return Matrix4 result
    ///
    static Mat4 translate(const Vec3& position) {
        Mat4 result = Mat4::identity();
        result.r1[3] = position.x;
        result.r2[3] = position.y;
        result.r3[3] = position.z;
        return result;
    }

    ///
    /// @brief Scale matrix
    /// @return Matrix4 result
    ///
    static Mat4 scale(const Vec3& scale) {
        Mat4 result = Mat4::identity();
        result.r1[0] = scale.x;
        result.r2[1] = scale.y;
        result.r3[2] = scale.z;
        return result;
    }

    ///
    /// @brief RotateX matrix
    /// @return Matrix4 result
    ///
    static Mat4 rotateX(float angle) {
        Mat4 result = Mat4::identity();
        result.r2[1] = std::cos(angle);
        result.r2[2] = -std::sin(angle);
        result.r3[1] = std::sin(angle);
        result.r3[2] = std::cos(angle);
        return result;
    }

    ///
    /// @brief RotateY matrix
    /// @return Matrix4 result
    ///
    static Mat4 rotateY(float angle) {
        Mat4 result = Mat4::identity();
        result.r1[0] = std::cos(angle);
        result.r1[2] = std::sin(angle);
        result.r3[0] = -std::sin(angle);
        result.r3[2] = std::cos(angle);
        return result;
    }

    ///
    /// @brief RotateZ matrix
    /// @return Matrix4 result
    ///
    static Mat4 rotateZ(float angle) {
        Mat4 result = Mat4::identity();
        result.r1[0] = std::cos(angle);
        result.r1[1] = -std::sin(angle);
        result.r2[0] = std::sin(angle);
        result.r2[1] = std::cos(angle);
        return result;
    }

    ///
    /// @brief Perspective projection matrix (LH)
    /// @param fov Field of View
    /// @param aspect Aspect ratio
    /// @param zNear Near plane clip
    /// @param zFar Far plane clip
    /// @return Matrix4
    ///
    static Mat4 perspective(float fov, float aspect, float zNear, float zFar) {
        if (std::abs(aspect - std::numeric_limits<float>::epsilon()) <= 0.f) {
            return Mat4::identity();
        }

        const float halfFOV = std::tan(fov * 0.5f);
        Mat4 matrix;
        matrix.r1[0] = 1.f / (aspect * halfFOV);
        matrix.r2[1] = 1.f / halfFOV;
        matrix.r3[2] = zFar / (zFar - zNear);
        matrix.r3[3] = 1.f;
        matrix.r4[2] = -(zFar * zNear) / (zFar - zNear);
        return matrix;
    }

    ///
    /// @brief Orthographic projection matrix (LH)
    /// @param left Left
    /// @param right Right
    /// @param bottom Bottom
    /// @param top Top
    /// @param zNear Near plane clip
    /// @param zFar Far plane clip
    /// @return Matrix4
    ///
    static Mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
        Mat4 matrix = Mat4::identity();
        matrix.r1[0] = 0.5f * (right - left);
        matrix.r2[1] = 0.5f * (top - bottom);
        matrix.r3[2] = 1.f / (zFar - zNear);
        matrix.r4[0] = -(right + left) / (right - left);
        matrix.r4[1] = -(top + bottom) / (top - bottom);
        matrix.r4[2] = -zNear / (zFar - zNear);
        return matrix;
    }
};