#pragma once

#include "entity.hpp"

#include <glm/glm.hpp>

class Camera : public Entity
{
    glm::vec4 m_clearColor;
    glm::vec3 m_target;
    float m_fov;
    float m_zNear;
    float m_zFar;
    float m_aspectRatio;

private:
    friend class glTFLoader;

public:
    Camera(const std::string&, float);
    ~Camera() = default;

public:
    void backgroundColor(const glm::vec4&);
    void target(const glm::vec3&);
    void aspectRatio(float);
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    void draw(const Shader&, const glm::mat4& = glm::mat4(1)) const override;
};