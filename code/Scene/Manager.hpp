#pragma once

#include "Node.hpp"

#include <memory>

class SceneManager final
{
    SceneNode* m_rootNode;

private:
    SceneManager() : m_rootNode{ new SceneNode } {}
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    ~SceneManager();

public:
    static SceneManager& instance();

public:
    void render(const Shader&) const;

public:
    inline SceneNode* getRoot() const { return m_rootNode; }
};