#pragma once

#include "ay.h"

#include <glad/glad.h>
#include <string>
#include <functional>
#include <glm/glm.hpp>

class Shader final
{
    GLuint m_id;
    GLenum m_type;

public:
    friend class ShaderProgram;

    AY_API Shader(GLenum);
    AY_API ~Shader();

    AY_API void fromFile(const std::string&) const;
    AY_API void fromMemory(const std::string&) const;
};

class ShaderProgram final
{
    GLuint m_id;
    const Shader& m_vertex;
    const Shader& m_fragment;

public:
    AY_API ShaderProgram(const Shader&, const Shader&);
    AY_API ~ShaderProgram();

    AY_API bool build() const;
    AY_API void use() const;
    AY_API void reset() const;

    AY_API void uniform(const std::string&, glm::mat4) const;
    AY_API void uniform(const std::string&, glm::mat3) const;
    AY_API void uniform(const std::string&, glm::vec4) const;
    AY_API void uniform(const std::string&, glm::vec3) const;
    AY_API void uniform(const std::string&, glm::vec2) const;
    AY_API void uniform(const std::string&, GLfloat) const;
    AY_API void uniform(const std::string&, GLint) const;
};