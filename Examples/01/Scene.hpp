#pragma once

#include "Scene/Node.hpp"

class MainScene final
{
    SceneNode* m_root;

public:
    MainScene();

    inline SceneNode* getNode() const { return m_root; }
};