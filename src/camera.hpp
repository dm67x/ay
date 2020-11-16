#pragma once

#include "math.hpp"
#include "context.hpp"
#include <string>

class Camera {
protected:
    Context* ctx;
    float zNear;
    float zFar;
    Vec3 position;
    Vec3 front;
    Vec3 up;
    Vec3 right;

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
        up(0.f, 1.f, 0.f),
        right(1.f, 0.f, 0.f)
    {
    }

public:
    /// 
    /// @brief Destructor
    /// 
    virtual ~Camera() {}

    /// 
    /// @brief Update (move, render, ...)
    /// @param deltaTime Elapsed time between each frame
    /// 
    virtual void update(float deltaTime) = 0;
};

class PerspectiveCamera : public Camera {
    float fov;

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
};

class OrthographicCamera : public Camera {
    float left;
    float right;
    float bottom;
    float top;

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
};

struct FreeCamera : public PerspectiveCamera {
    float speed;
    
    /// 
    /// @brief Constructor
    /// @param ctx Context
    /// @param fov Field of view
    /// @param near Near clip
    /// @param far Far clip
    /// 
    FreeCamera(Context* ctx, float fov, float near, float far)
        : PerspectiveCamera(ctx, fov, near, far), speed(8.f)
    {
    }

    /// 
    /// @brief Update method
    /// @param deltaTime Elapsed time between each frame
    /// 
    void update(float deltaTime) override;
};