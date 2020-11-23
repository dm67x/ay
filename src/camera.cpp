#include "camera.hpp"
#include "context.hpp"
#include "window.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace ay
{
    void PerspectiveCamera::update(f32 deltaTime) {
        (void)deltaTime;
        auto& window = ctx->getWindow();
        const f32 aspect = static_cast<f32>(window.getSize().first) / static_cast<f32>(window.getSize().second);

        glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
        glm::mat4 view = glm::lookAt(position, position + front, up);
        ctx->shaderUniform("projectionMatrix", projection);
        ctx->shaderUniform("viewMatrix", view);
        ctx->shaderUniform("cameraPosition", position);
    }

    void OrthographicCamera::update(f32 deltaTime) {
        (void)deltaTime;

        glm::mat4 projection = glm::ortho(left, right, bottom, top, zNear, zFar);
        glm::mat4 view = glm::translate(glm::mat4(1.f), position);
        ctx->shaderUniform("projectionMatrix", projection);
        ctx->shaderUniform("viewMatrix", view);
        ctx->shaderUniform("cameraPosition", position);
    }

    void FreeCamera::update(f32 deltaTime) {
        auto& window = ctx->getWindow();
        float t = window.isKeyPressed(GLFW_KEY_W) ? 1.f : (window.isKeyPressed(GLFW_KEY_S) ? -1.f : 0.f);
        float r = window.isKeyPressed(GLFW_KEY_A) ? -1.f : (window.isKeyPressed(GLFW_KEY_D) ? 1.f : 0.f);

        // mouse position
        auto mousePosition = window.getMousePosition();
        float xoffset = mousePosition.first - lastMousePosition.x;
        float yoffset = lastMousePosition.y - mousePosition.second;
        lastMousePosition = glm::vec2(mousePosition.first, mousePosition.second);

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;
        pitch = std::max(std::min(pitch, 89.f), -89.f);

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);

        glm::vec3 right = glm::cross(front, up);
        position += (front * t + right * r) * (speed * deltaTime);
        PerspectiveCamera::update(deltaTime);
    }
}