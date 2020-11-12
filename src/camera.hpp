#pragma once

#include "math.hpp"

class Camera {
    float zNear;
    float zFar;
    Vec3 position;
    Vec3 viewDirection;
    Vec3 up;

protected:
    Camera(float near, float far) 
        : zNear(near), zFar(far), position(), viewDirection(0.f, 0.f, 1.f), up(0.f, 1.f, 0.f)
    {
    }

public:
    virtual void move() = 0;
    virtual void update() = 0;
    virtual void renderToTexture() = 0;
};

class PerspectiveCamera : public Camera {
    float fov;
    float aspect;

public:
    PerspectiveCamera(float fov, float aspect, float near, float far)
        : fov(fov), aspect(aspect), Camera(near, far)
    {
    }
};

class OrthographicCamera : public Camera {
    float left;
    float right;
    float bottom;
    float top;

public:
    OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
        : left(left), right(right), bottom(bottom), top(top), Camera(near, far)
    {
    }
};