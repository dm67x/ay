#pragma once

#include "Node.hpp"
#include "Ay.hpp"

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

    inline SceneNode* getRoot() const { return m_rootNode; }
};