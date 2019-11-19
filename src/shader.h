#pragma once

#include <glad/glad.h>
#include <string>
#include <functional>

struct Shader final
{
    friend struct ShaderProgram;

    Shader(GLenum);
    ~Shader();

    void FromFile(const std::string&) const;
    void FromMemory(const std::string&) const;

private:
    GLuint m_id;
    GLenum m_type;
};

struct ShaderProgram final
{
    ShaderProgram(const Shader&, const Shader&);
    ~ShaderProgram();

    bool Build() const;
    void Use() const;
    void Reset() const;
    void Uniform(const std::string&, std::function<void(GLint)>) const;

private:
    GLuint m_id;
    const Shader& m_vertex;
    const Shader& m_fragment;
};