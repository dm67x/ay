#pragma once

#include "render/shader.h"

#include <vector>
#include <glm/glm.hpp>

class SceneEntity;

class SceneNode
{
    std::vector<SceneNode*> m_nodes;
    SceneEntity* m_entity;
    glm::vec3 m_position;

public:
    SceneNode();
    virtual ~SceneNode();

public:
    SceneNode* operator[](int) const;

public:
    SceneNode* create();
    void attach(SceneEntity*);
    void position(glm::vec3);
    virtual void draw(const Shader&) const;

public:
    inline glm::vec3 position() const { return m_position; }
    inline SceneEntity* entity() const { return m_entity; }
};