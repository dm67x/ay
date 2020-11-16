#include "camera.hpp"
#include "window.hpp"

void PerspectiveCamera::update(float deltaTime) {
    (void)deltaTime;
    auto& window = ctx->getWindow();
    const float aspect = static_cast<float>(window.getSize().first) / static_cast<float>(window.getSize().second);

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

void FreeCamera::update(float deltaTime) {
    auto& window = ctx->getWindow();
    float t = window.isKeyPressed(GLFW_KEY_W) ? -1.f : (window.isKeyPressed(GLFW_KEY_S) ? 1.f : 0.f);
    float r = window.isKeyPressed(GLFW_KEY_A) ? 1.f : (window.isKeyPressed(GLFW_KEY_D) ? -1.f : 0.f);

    position += (front * t + right * r) * (speed * deltaTime);
    PerspectiveCamera::update(deltaTime);
}