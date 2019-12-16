#include "Scene/Manager.hpp"

SceneManager::~SceneManager()
{
    delete m_rootNode;
}

SceneManager& SceneManager::instance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::render(const ShaderProgram& program) const
{
    m_rootNode->draw(program);
}