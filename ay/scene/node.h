#pragma once

#include "ay.h"

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

    AY_API SceneNode* create();
    AY_API void attach(SceneEntity*);
    AY_API void position(glm::vec3);
    AY_API void draw() const;

    inline AY_API glm::vec3 position() const { return m_position; }
};