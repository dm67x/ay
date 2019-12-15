#pragma once

#include "Ay.hpp"

#include <glad/glad.h>
#include <string>

struct Texture2DParameter
{
    GLint wrap_s;
    GLint wrap_t;
    GLint min;
    GLint mag;
};

class Texture2D final
{
    GLuint m_id;
    GLuint m_width;
    GLuint m_height;
    Texture2DParameter m_parameter;

public:
    AY_API Texture2D(const Texture2DParameter&);
    AY_API ~Texture2D();

    AY_API void create(const GLuint, const GLuint) const;
    AY_API bool load(const std::string&) const;
    AY_API void bind(GLuint = 0) const;
    AY_API void unbind() const;
};