#include "camera.hpp"

void PerspectiveCamera::update(float deltaTime) {
    (void)deltaTime;
    Mat4 projection = Mat4::perspective(radians(fov), aspect, zNear, zFar);
    ctx->shaderUniform("projectionViewMatrix", projection.transpose());
}

void OrthographicCamera::update(float deltaTime) {
    (void)deltaTime;
    Mat4 projection = Mat4::ortho(left, right, bottom, top, zNear, zFar);
    ctx->shaderUniform("projectionViewMatrix", projection);
    ctx->shaderUniform("cameraPosition", position);
}