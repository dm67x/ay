#pragma once

#include "math.hpp"
#include <string>

class Context;

class Camera {
    float zNear;
    float zFar;
    Vec3 position;
    Vec3 viewDirection;
    Vec3 up;
    Context* ctx;

protected:
    Camera(Context* ctx, float near, float far) 
        : ctx(ctx), zNear(near), zFar(far), position(), viewDirection(0.f, 0.f, 1.f), up(0.f, 1.f, 0.f)
    {
    }

public:
    virtual void move() = 0;
    virtual void update() = 0;
    virtual void renderToTexture(const std::string& name) = 0;
};

class PerspectiveCamera : public Camera {
    float fov;
    float aspect;

public:
    PerspectiveCamera(Context* ctx, float fov, float aspect, float near, float far)
        : fov(fov), aspect(aspect), Camera(ctx, near, far)
    {
    }
};

class OrthographicCamera : public Camera {
    float left;
    float right;
    float bottom;
    float top;

public:
    OrthographicCamera(Context* ctx, float left, float right, float bottom, float top, float near, float far)
        : left(left), right(right), bottom(bottom), top(top), Camera(ctx, near, far)
    {
    }
};