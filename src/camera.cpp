#include "camera.hpp"
#include "window.hpp"

void PerspectiveCamera::update(float deltaTime) {
    (void)deltaTime;
    auto& window = ctx->getWindow();
    auto front = window.isKeyPressed(GLFW_KEY_W) ? -1 : (window.isKeyPressed(GLFW_KEY_S) ? 1 : 0);
    auto left = window.isKeyPressed(GLFW_KEY_A) ? 1 : (window.isKeyPressed(GLFW_KEY_D) ? -1 : 0);

    const float speed = 8.f;
    position.z += speed * deltaTime * front;
    position.x += speed * deltaTime * left;

    Mat4 projection = Mat4::perspective(radians(fov), aspect, zNear, zFar);
    ctx->shaderUniform("projectionMatrix", projection.transpose());
    ctx->shaderUniform("viewMatrix", Mat4::translate(position));
}

void OrthographicCamera::update(float deltaTime) {
    (void)deltaTime;
    Mat4 projection = Mat4::ortho(left, right, bottom, top, zNear, zFar);
    ctx->shaderUniform("projectionMatrix", projection.transpose());
    ctx->shaderUniform("viewMatrix", Mat4::identity());
}