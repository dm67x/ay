#include "shader.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>

Shader::Shader(GLenum type)
    : m_id{ 0 },
    m_type{ type }
{
    m_id = glCreateShader(type);
}

Shader::~Shader()
{
    glDeleteShader(m_id);
}

void Shader::FromFile(const std::string& source) const
{
    std::ifstream file(source);
    if (file.is_open()) {
        std::string str((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        FromMemory(str);
        file.close();
    }
}

void Shader::FromMemory(const std::string& source) const
{
    const GLchar* src = static_cast<const GLchar*>(source.data());
    const GLint size = static_cast<const GLint>(source.size());
    glShaderSource(m_id, 1, &src, &size);
    glCompileShader(m_id);
    GLint status = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::vector<GLchar> log;
        GLint logSize;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &logSize);
        log.resize(logSize);
        glGetShaderInfoLog(m_id, logSize, nullptr, &log[0]);
        std::string logStr(log.begin(), log.end());
        std::cerr << logStr << std::endl;
    }
}

ShaderProgram::ShaderProgram(const Shader& vert, const Shader& frag)
    : m_id{ 0 },
    m_vertex{ vert },
    m_fragment{ frag }
{
    m_id = glCreateProgram();
    Build();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_id);
}

bool ShaderProgram::Build() const
{
    glAttachShader(m_id, m_vertex.m_id);
    glAttachShader(m_id, m_fragment.m_id);

    glLinkProgram(m_id);
    GLint status = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::vector<GLchar> log;
        GLint logSize;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logSize);
        log.resize(logSize);
        glGetProgramInfoLog(m_id, logSize, nullptr, &log[0]);
        std::string logStr(log.begin(), log.end());
        std::cerr << logStr << std::endl;
        return false;
    }

    glDetachShader(m_id, m_vertex.m_id);
    glDetachShader(m_id, m_fragment.m_id);
    return true;
}

void ShaderProgram::Use() const
{
    glUseProgram(m_id);
}

void ShaderProgram::Reset() const
{
    glUseProgram(0);
}

void ShaderProgram::Uniform(
    const std::string& name,
    std::function<void(GLint)> function) const
{
    GLint location = glGetUniformLocation(m_id, name.c_str());
    function(location);
}