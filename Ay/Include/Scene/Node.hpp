#pragma once

#include "Ay.hpp"
#include "Render/Shader.hpp"

#include <vector>
#include <glm/glm.hpp>

class SceneEntity;

class SceneNode
{
    std::vector<SceneNode*> m_nodes;
    SceneEntity* m_entity;
    glm::vec3 m_position;

public:
    AY_API SceneNode();
    AY_API virtual ~SceneNode();
    AY_API SceneNode* operator[](int) const;

    AY_API SceneNode* create();
    AY_API void attach(SceneEntity*);
    AY_API void position(glm::vec3);
    AY_API virtual void draw(const ShaderProgram&) const;

    inline glm::vec3 position() const { return m_position; }
    inline SceneEntity* entity() const { return m_entity; }
};