#pragma once

#include "node.h"

#include <memory>

class SceneManager final
{
    SceneNode m_rootNode;

public:
    SceneManager();
    ~SceneManager() = default;

public:
    void render(const Shader&) const;

public:
    inline SceneNode& getRoot() { return m_rootNode; }
};