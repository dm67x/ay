#include "texture2d.h"

#include <iostream>
#include <stb_image.h>

Texture2D::Texture2D(const Texture2DParameter& params)
    : m_id{ 0 },
    m_width{ 0 },
    m_height{ 0 },
    m_parameter{ params }
{
    glGenTextures(1, &m_id);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_id);
}

void Texture2D::Create(const GLuint width, const GLuint height) const
{
    Bind();

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        m_parameter.min);
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        m_parameter.mag);
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        m_parameter.wrap_s);
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        m_parameter.wrap_t);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr);

    Unbind();
}

bool Texture2D::Load(const std::string& filename) const
{
    int width, height;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* image = stbi_load(
        filename.c_str(),
        &width, &height, nullptr,
        STBI_rgb_alpha);

    if (!image) {
        std::cerr << "error when loading image: " << filename << std::endl;
        return false;
    }

    Bind();

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        m_parameter.min);
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        m_parameter.mag);
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        m_parameter.wrap_s);
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        m_parameter.wrap_t);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image);

    Unbind();

    stbi_image_free(image);
    return true;
}

void Texture2D::Bind(GLuint unit) const
{
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}