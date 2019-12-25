#include "shader.hpp"
#include "log.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>

void Shader::ShaderInst::fromMemory(const std::string& source) const
{
    const GLchar* src = static_cast<const GLchar*>(source.data());
    const GLint size = static_cast<const GLint>(source.size());
    glShaderSource(id, 1, &src, &size);
    glCheckError();
    glCompileShader(id);
    glCheckError();
    GLint status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    glCheckError();
    if (status == GL_FALSE) {
        std::vector<GLchar> log;
        GLint logSize;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);
        glCheckError();
        log.resize(logSize);
        glGetShaderInfoLog(id, logSize, nullptr, &log[0]);
        glCheckError();
        std::string logStr(log.begin(), log.end());
        std::cerr << logStr << std::endl;
    }
}

void Shader::ShaderInst::fromFile(const std::string& source) const
{
    std::ifstream file(source);
    if (file.is_open()) {
        std::string str((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        fromMemory(str);
        file.close();
    }
}

// Shader program
Shader::Shader()
    : m_id{ 0 },
    m_vertex{ ShaderInst{ 0, GL_VERTEX_SHADER } },
    m_fragment{ ShaderInst{ 0, GL_FRAGMENT_SHADER } }
{
    m_id = glCreateProgram();
    glCheckError();
    m_vertex.id = glCreateShader(m_vertex.type);
    glCheckError();
    m_fragment.id = glCreateShader(m_fragment.type);
    glCheckError();
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
    glCheckError();
}

bool Shader::load(const std::string& filename) const
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    enum Mode { NONE, VERTEX, FRAGMENT };
    Mode currentMode = Mode::NONE;

    std::string vertex = "";
    std::string fragment = "";

    for (std::string line; std::getline(file, line); ) {
        // #vertex
        if (line == "#vertex") {
            if (currentMode != Mode::NONE) return false;
            currentMode = Mode::VERTEX;
            continue;
        }
        else if (line == "#fragment") {
            if (currentMode != Mode::NONE) return false;
            currentMode = Mode::FRAGMENT;
            continue;
        }
        else if (line == "#end") {
            currentMode = Mode::NONE;
            continue;
        }

        if (currentMode == Mode::VERTEX) {
            vertex += line + "\n";
        }
        else if (currentMode == Mode::FRAGMENT) {
            fragment += line + "\n";
        }
    }

    if (vertex.empty() || fragment.empty()) {
        return false;
    }

    m_vertex.fromMemory(vertex);
    m_fragment.fromMemory(fragment);

    file.close();
    return build();
}

bool Shader::build() const
{
    glAttachShader(m_id, m_vertex.id);
    glCheckError();
    glAttachShader(m_id, m_fragment.id);
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

    glDetachShader(m_id, m_vertex.id);
    glCheckError();
    glDetachShader(m_id, m_fragment.id);
    glCheckError();

    return true;
}

void Shader::use() const
{
    glUseProgram(m_id);
    glCheckError();
}

void Shader::reset() const
{
    glUseProgram(0);
    glCheckError();
}

void Shader::uniform(const std::string& name, glm::mat4 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
    glCheckError();
}

void Shader::uniform(const std::string& name, glm::mat3 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniformMatrix3fv(loc, 1, GL_FALSE, &value[0][0]);
    glCheckError();
}

void Shader::uniform(const std::string& name, glm::vec4 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform4fv(loc, 1, &value[0]);
    glCheckError();
}

void Shader::uniform(const std::string& name, glm::vec3 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform3fv(loc, 1, &value[0]);
    glCheckError();
}

void Shader::uniform(const std::string& name, glm::vec2 value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform2fv(loc, 1, &value[0]);
    glCheckError();
}

void Shader::uniform(const std::string& name, GLfloat value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform1f(loc, value);
    glCheckError();
}

void Shader::uniform(const std::string& name, GLint value) const
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    glCheckError();
    glUniform1i(loc, value);
    glCheckError();
}