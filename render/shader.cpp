#include "shader.h"
#include "main/log.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>

Shader::Shader(GLenum type)
    : m_id{ 0 },
    m_type{ type }
{
    m_id = glCreateShader(type);
    glCheckError();
}

Shader::~Shader()
{
    glDeleteShader(m_id);
    glCheckError();
}

void Shader::fromFile(const std::string& source) const
{
    std::ifstream file(source);
    if (file.is_open()) {
        std::string str((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        fromMemory(str);
        file.close();
    }
}

void Shader::fromMemory(const std::string& source) const
{
    const GLchar* src = static_cast<const GLchar*>(source.data());
    const GLint size = static_cast<const GLint>(source.size());
    glShaderSource(m_id, 1, &src, &size);
    glCheckError();
    glCompileShader(m_id);
    glCheckError();
    GLint status = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
    glCheckError();
    if (status == GL_FALSE) {
        std::vector<GLchar> log;
        GLint logSize;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &logSize);
        glCheckError();
        log.resize(logSize);
        glGetShaderInfoLog(m_id, logSize, nullptr, &log[0]);
        glCheckError();
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
    glCheckError();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_id);
    glCheckError();
}

bool ShaderProgram::build() const
{
    glAttachShader(m_id, m_vertex.m_id);
    glCheckError();
    glAttachShader(m_id, m_fragment.m_id);
    glCheckError();

    glLinkProgram(m_id);
    glCheckError();
    GLint status = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &status);
    glCheckError();
    if (status == GL_FALSE) {
        std::vector<GLchar> log;
        GLint logSize;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logSize);
        glCheckError();
        log.resize(logSize);
        glGetProgramInfoLog(m_id, logSize, nullptr, &log[0]);
        glCheckError();
        std::string logStr(log.begin(), log.end());
        std::cerr << logStr << std::endl;
        return false;
    }

    glDetachShader(m_id, m_vertex.m_id);
    glCheckError();
    glDetachShader(m_id, m_fragment.m_id);
    glCheckError();

    return true;
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
    glCheckError();
}

void ShaderProgram::reset() const
{
    glUseProgram(0);
    glCheckError();
}

void ShaderProgram::uniform(
    const std::string& name,
    std::function<void(GLint)> function) const
{
    GLint location = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    function(location);
}