#include "Render/Texture2D.hpp"
#include "Log.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::Texture2D(const Texture2DParameter& params)
    : m_id{ 0 },
    m_width{ 0 },
    m_height{ 0 },
    m_parameter{ params }
{
    glGenTextures(1, &m_id);
    glCheckError();
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_id);
    glCheckError();
}

void Texture2D::create(const GLuint width, const GLuint height) const
{
    bind();

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        m_parameter.min);
    glCheckError();
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        m_parameter.mag);
    glCheckError();
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        m_parameter.wrap_s);
    glCheckError();
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        m_parameter.wrap_t);
    glCheckError();

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
    glCheckError();

    unbind();
}

bool Texture2D::load(const std::string& filename) const
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

    bind();

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        m_parameter.min);
    glCheckError();
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        m_parameter.mag);
    glCheckError();
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        m_parameter.wrap_s);
    glCheckError();
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        m_parameter.wrap_t);
    glCheckError();

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
    glCheckError();

    unbind();

    stbi_image_free(image);
    return true;
}

void Texture2D::bind(GLuint unit) const
{
    glActiveTexture(unit);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, m_id);
    glCheckError();
}

void Texture2D::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();
}