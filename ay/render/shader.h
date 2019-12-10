#pragma once

#include "ay.h"

#include <glad/glad.h>
#include <string>
#include <functional>
#include <glm/glm.hpp>

template<GLenum T>
class Shader final
{
    GLuint m_id;

    friend class ShaderProgram;

public:
    AY_API Shader();
    AY_API ~Shader();

    AY_API void fromFile(const std::string&) const;
    AY_API void fromMemory(const std::string&) const;
};

class ShaderProgram final
{
    GLuint m_id;
    const Shader<GL_VERTEX_SHADER>& m_vertex;
    const Shader<GL_FRAGMENT_SHADER>& m_fragment;

    friend class Material;

public:
    AY_API ShaderProgram(const Shader<GL_VERTEX_SHADER>&, 
        const Shader<GL_FRAGMENT_SHADER>&);
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