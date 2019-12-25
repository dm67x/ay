#pragma once

#include "entity.hpp"

#include <glm/glm.hpp>

class Camera : public Entity
{
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
    void target(const glm::vec3&);
    void aspectRatio(float);
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    void draw(const Shader&, const Transform& = Transform()) const override;
};