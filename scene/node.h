#pragma once

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

    SceneNode* create();
    void attach(SceneEntity*);
    void position(glm::vec3);
    void draw() const;

    inline glm::vec3 position() const { return m_position; }
};