#pragma once

#include "node.h"
#include "ay.h"

#include <memory>

class SceneManager final
{
    SceneNode* m_rootNode;

    SceneManager() : m_rootNode{ new SceneNode } {}
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    AY_API ~SceneManager();
    AY_API static SceneManager& instance();

    AY_API void render() const;

    inline AY_API SceneNode* getRoot() const { return m_rootNode; }
};