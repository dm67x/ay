#pragma once

#include <glad/glad.h>
#include <string>
#include <functional>
#include <glm/glm.hpp>

class Shader final
{
    GLuint m_id;

    struct ShaderInst
    {
        GLuint id;
        GLenum type;

        void fromMemory(const std::string&) const;
        void fromFile(const std::string&) const;
    };

    ShaderInst m_vertex;
    ShaderInst m_fragment;

private:
    friend class Material;

public:
    Shader();
    ~Shader();

public:
    bool load(const std::string&) const;
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

public:
    inline ShaderInst& vertex() { return m_vertex; }
    inline ShaderInst& fragment() { return m_fragment; }
};