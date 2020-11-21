#pragma once

#include "context.hpp"
#include <string>
#include <glm/glm.hpp>

class Camera {
public:
    /// 
    /// @brief Destructor
    /// 
    virtual ~Camera() = default;

    /// 
    /// @brief Update (move, render, ...)
    /// @param deltaTime Elapsed time between each frame
    /// 
    virtual void update(float deltaTime) = 0;

protected:
    /// 
    /// @brief Constructor
    /// @param ctx Context
    /// @param near Near clip
    /// @param Far clip
    /// 
    Camera(Context* ctx, float near, float far) 
        : ctx(ctx), 
        zNear(near), 
        zFar(far), 
        position(), 
        front(0.f, 0.f, 1.f), 
        up(0.f, 1.f, 0.f)
    {
    }

protected:
    Context* ctx;
    float zNear;
    float zFar;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
};

class PerspectiveCamera : public Camera {
public:
    /// 
    /// @brief Constructor
    /// @param ctx Context
    /// @param fov Field of view
    /// @param near Near clip
    /// @param far Far clip
    /// 
    PerspectiveCamera(Context* ctx, float fov, float near, float far)
        : Camera(ctx, near, far), fov(fov)
    {
    }

    /// 
    /// @brief Update method
    /// @param deltaTime Elapsed time between each frame
    /// 
    virtual void update(float deltaTime) override;

private:
    float fov;
};

class OrthographicCamera : public Camera {
public:
    /// 
    /// @brief Constructor
    /// @param ctx Context
    /// @param left Left
    /// @param right Right
    /// @param bottom Bottom
    /// @param top Top
    /// @param near Near clip
    /// @param far Far clip
    /// 
    OrthographicCamera(Context* ctx, float left, float right, float bottom, float top, float near, float far)
        : Camera(ctx, near, far), left(left), right(right), bottom(bottom), top(top)
    {
    }

    /// 
    /// @brief Update method
    /// @param deltaTime Elapsed time between each frame
    /// 
    virtual void update(float deltaTime) override;

private:
    float left;
    float right;
    float bottom;
    float top;
};

class FreeCamera : public PerspectiveCamera { 
public:
    /// 
    /// @brief Constructor
    /// @param ctx Context
    /// @param fov Field of view
    /// @param near Near clip
    /// @param far Far clip
    /// 
    FreeCamera(Context* ctx, float fov, float near, float far)
        : PerspectiveCamera(ctx, fov, near, far), speed(8.f), yaw(0.f), pitch(0.f), lastMousePosition()
    {
    }

    /// 
    /// @brief Update method
    /// @param deltaTime Elapsed time between each frame
    /// 
    void update(float deltaTime) override;

private:
    float speed;
    float yaw;
    float pitch;
    glm::vec2 lastMousePosition;
};