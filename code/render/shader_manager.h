#pragma once

#include <map>
#include <string>
#include <memory>

class Shader;

class ShaderManager final
{
    std::map<std::string, std::unique_ptr<Shader>> m_shaders;

public:
    ShaderManager();
    ~ShaderManager() = default;

public:
    Shader* operator[](const std::string&);

public:
    Shader* create(const std::string&);
};