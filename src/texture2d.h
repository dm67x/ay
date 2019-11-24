#pragma once

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
    Texture2D(const Texture2DParameter&);
    ~Texture2D();

    void create(const GLuint, const GLuint) const;
    bool load(const std::string&) const;
    void bind(GLuint = 0) const;
    void unbind() const;
};