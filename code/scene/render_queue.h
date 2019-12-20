#pragma once

#include <queue>
#include <memory>

class SceneEntity;

class RenderQueue
{
    std::queue<std::shared_ptr<SceneEntity>> m_queue;

public:
    RenderQueue();

public:
    SceneEntity* get() const;

public:
    void add(const SceneEntity&);
};