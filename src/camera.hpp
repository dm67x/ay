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

    /// 
    /// @brief Render to a Texture2D
    /// @param name Texture name
    /// 
    virtual void renderToTexture(const std::string& name) {
        (void)name;
        /*Texture2D* texture = ctx->texture2DGet(name);
        if (!texture) {
            spdlog::error("Texture [{}] didn't exist inside the context. Create the texture before rendering to it.", name);
            return;
        }
        
        Framebuffer* framebuffer = ctx->framebufferGet("camera");
        if (!framebuffer) {
            Renderbuffer* rb = ctx->renderbufferNew("camera", );
            framebuffer = ctx->framebufferNew("camera", { texture }, rb);
        }*/
    }

    /// 
    /// @brief Set camera aspect ratio (for PerspectiveCamera)
    /// @param aspectRatio Aspect ratio
    /// 
    virtual void setAspectRatio(float aspectRatio) {
        (void)aspectRatio;
    }
};

class PerspectiveCamera : public Camera {
    float fov;
    float aspect;

public:
    PerspectiveCamera(Context* ctx, float fov, float aspect, float near, float far)
        : Camera(ctx, near, far), fov(fov), aspect(aspect)
    {
    }

    void update(float deltaTime) override;

    inline void setAspectRatio(float aspectRatio) override { 
        this->aspect = aspectRatio;
    }
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

    void update(float deltaTime) override;
};