#pragma once

#include "node.h"

#include <memory>

class SceneManager final
{
    SceneNode* m_rootNode;

    SceneManager() : m_rootNode{ new SceneNode } {}
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    ~SceneManager();
    static SceneManager& instance();

    void render() const;

    inline SceneNode* getRoot() const { return m_rootNode; }
};