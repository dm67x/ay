#pragma once

#include "types.hpp"
#include <string>
#include <glm/glm.hpp>

namespace ay
{
    class Context;

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
        virtual void update(f32 deltaTime) = 0;

    protected:
        /// 
        /// @brief Constructor
        /// @param ctx Context
        /// @param near Near clip
        /// @param Far clip
        /// 
        Camera(Context* ctx, f32 near, f32 far)
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
        f32 zNear;
        f32 zFar;
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
        PerspectiveCamera(Context* ctx, f32 fov, f32 near, f32 far)
            : Camera(ctx, near, far), fov(fov)
        {
        }

        /// 
        /// @brief Update method
        /// @param deltaTime Elapsed time between each frame
        /// 
        virtual void update(f32 deltaTime) override;

    private:
        f32 fov;
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
        OrthographicCamera(Context* ctx, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
            : Camera(ctx, near, far), left(left), right(right), bottom(bottom), top(top)
        {
        }

        /// 
        /// @brief Update method
        /// @param deltaTime Elapsed time between each frame
        /// 
        virtual void update(f32 deltaTime) override;

    private:
        f32 left;
        f32 right;
        f32 bottom;
        f32 top;
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
        FreeCamera(Context* ctx, f32 fov, f32 near, f32 far)
            : PerspectiveCamera(ctx, fov, near, far), speed(8.f), yaw(0.f), pitch(0.f), lastMousePosition()
        {
        }

        /// 
        /// @brief Update method
        /// @param deltaTime Elapsed time between each frame
        /// 
        void update(f32 deltaTime) override;

    private:
        f32 speed;
        f32 yaw;
        f32 pitch;
        glm::vec2 lastMousePosition;
    };
}