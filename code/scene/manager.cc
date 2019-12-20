#include "manager.h"

SceneManager::SceneManager()
    : m_rootNode{}
{
}

void SceneManager::render(const Shader& program) const
{
    m_rootNode.draw(program);
}