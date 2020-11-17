#include "camera.hpp"
#include "window.hpp"
#include <glm/gtc/matrix_transform.hpp>

void PerspectiveCamera::update(float deltaTime) {
    (void)deltaTime;
    auto& window = ctx->getWindow();
    const float aspect = static_cast<float>(window.getSize().first) / static_cast<float>(window.getSize().second);

    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    glm::mat4 view = glm::translate(glm::mat4(1.f), position);
    ctx->shaderUniform("projectionMatrix", projection);
    ctx->shaderUniform("viewMatrix", view);
    ctx->shaderUniform("cameraPosition", position);
}

void OrthographicCamera::update(float deltaTime) {
    (void)deltaTime;

    glm::mat4 projection = glm::ortho(left, right, bottom, top, zNear, zFar);
    glm::mat4 view = glm::translate(glm::mat4(1.f), position);
    ctx->shaderUniform("projectionMatrix", projection);
    ctx->shaderUniform("viewMatrix", view);
    ctx->shaderUniform("cameraPosition", position);
}

void FreeCamera::update(float deltaTime) {
    auto& window = ctx->getWindow();
    float t = window.isKeyPressed(GLFW_KEY_W) ? 1.f : (window.isKeyPressed(GLFW_KEY_S) ? -1.f : 0.f);
    float r = window.isKeyPressed(GLFW_KEY_A) ? 1.f : (window.isKeyPressed(GLFW_KEY_D) ? -1.f : 0.f);

    position += (front * t + right * r) * (speed * deltaTime);
    PerspectiveCamera::update(deltaTime);
}