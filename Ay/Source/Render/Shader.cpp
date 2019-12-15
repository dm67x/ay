#include "Render/Shader.hpp"
#include "Log.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>

template<GLenum T>
Shader<T>::Shader()
    : m_id{ 0 }
{
    m_id = glCreateShader(T);
    glCheckError();
}

template<GLenum T>
Shader<T>::~Shader()
{
    glDeleteShader(m_id);
    glCheckError();
}

template<GLenum T>
void Shader<T>::fromFile(const std::string& source) const
{
    std::ifstream file(source);
    if (file.is_open()) {
        std::string str((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        fromMemory(str);
        file.close();
    }
}

template<GLenum T>
void Shader<T>::fromMemory(const std::string& source) const
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

// Instantiations for export
template Shader<GL_VERTEX_SHADER>::Shader();
template Shader<GL_FRAGMENT_SHADER>::Shader();
template Shader<GL_VERTEX_SHADER>::~Shader();
template Shader<GL_FRAGMENT_SHADER>::~Shader();
template void Shader<GL_VERTEX_SHADER>::fromFile(const std::string&) const;
template void Shader<GL_FRAGMENT_SHADER>::fromFile(const std::string&) const;
template void Shader<GL_VERTEX_SHADER>::fromMemory(const std::string&) const;
template void Shader<GL_FRAGMENT_SHADER>::fromMemory(const std::string&) const;

// Shader program
ShaderProgram::ShaderProgram(
    const Shader<GL_VERTEX_SHADER>& vert, 
    const Shader<GL_FRAGMENT_SHADER>& frag
)
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

void ShaderProgram::uniform(const std::string& name, glm::mat4 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::uniform(const std::string& name, glm::mat3 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniformMatrix3fv(loc, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::uniform(const std::string& name, glm::vec4 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform4fv(loc, 1, &value[0]);
}

void ShaderProgram::uniform(const std::string& name, glm::vec3 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform3fv(loc, 1, &value[0]);
}

void ShaderProgram::uniform(const std::string& name, glm::vec2 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform2fv(loc, 1, &value[0]);
}

void ShaderProgram::uniform(const std::string& name, GLfloat value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform1f(loc, value);
}

void ShaderProgram::uniform(const std::string& name, GLint value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform1i(loc, value);
}