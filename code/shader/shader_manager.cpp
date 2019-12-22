#include "shader_manager.hpp"
#include "shader.hpp"

ShaderManager::ShaderManager()
    : m_shaders{}
{
}

Shader* ShaderManager::operator[](const std::string& name)
{
    return m_shaders.at(name).get();
}

Shader* ShaderManager::create(const std::string& name)
{
    m_shaders.insert(std::make_pair(name, std::make_unique<Shader>()));
    return m_shaders.at(name).get();
}