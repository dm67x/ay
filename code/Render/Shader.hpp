#pragma once

#include <glad/glad.h>
#include <string>
#include <functional>
#include <glm/glm.hpp>

template<GLenum T>
class Shader final
{
    GLuint m_id;

private:
    friend class ShaderProgram;

public:
    Shader();
    ~Shader();

public:
    void fromFile(const std::string&) const;
    void fromMemory(const std::string&) const;
};

class ShaderProgram final
{
    GLuint m_id;
    const Shader<GL_VERTEX_SHADER>& m_vertex;
    const Shader<GL_FRAGMENT_SHADER>& m_fragment;

private:
    friend class Material;

public:
    ShaderProgram(const Shader<GL_VERTEX_SHADER>&,
        const Shader<GL_FRAGMENT_SHADER>&);
    ~ShaderProgram();

public:
    bool build() const;
    void use() const;
    void reset() const;
    void uniform(const std::string&, glm::mat4) const;
    void uniform(const std::string&, glm::mat3) const;
    void uniform(const std::string&, glm::vec4) const;
    void uniform(const std::string&, glm::vec3) const;
    void uniform(const std::string&, glm::vec2) const;
    void uniform(const std::string&, GLfloat) const;
    void uniform(const std::string&, GLint) const;
};