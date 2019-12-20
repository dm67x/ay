#include "Manager.hpp"

SceneManager::~SceneManager()
{
    delete m_rootNode;
}

SceneManager& SceneManager::instance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::render(const Shader& program) const
{
    m_rootNode->draw(program);
}