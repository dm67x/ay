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
    Vec3 viewDirection;
    Vec3 up;

protected:
    Camera(Context* ctx, float near, float far) 
        : ctx(ctx), 
        zNear(near), 
        zFar(far), 
        position(), 
        viewDirection(0.f, 0.f, 1.f), 
        up(0.f, 1.f, 0.f)
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
    PerspectiveCamera(Context* ctx, float fov, float near, float far)
        : Camera(ctx, near, far), fov(fov)
    {
    }

    virtual void update(float deltaTime) override;
};

class OrthographicCamera : public Camera {
    float left;
    float right;
    float bottom;
    float top;

public:
    OrthographicCamera(Context* ctx, float left, float right, float bottom, float top, float near, float far)
        : Camera(ctx, near, far), left(left), right(right), bottom(bottom), top(top)
    {
    }

    virtual void update(float deltaTime) override;
};

struct FreeCamera : public PerspectiveCamera {
    float speed;
    
    FreeCamera(Context* ctx, float fov, float near, float far)
        : PerspectiveCamera(ctx, fov, near, far), speed(8.f)
    {
    }

    void update(float deltaTime) override;
};