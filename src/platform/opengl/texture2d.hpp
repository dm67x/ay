#pragma once

#include "helper.hpp"

/// 
/// @brief TextureParameters
/// 
struct TextureParameters {
    using Helper = OpenGLHelper;

    Helper::TextureFiltering mag;
    Helper::TextureFiltering min;
    Helper::TextureWrap wrapS;
    Helper::TextureWrap wrapT;
    Helper::TextureFormat internalFormat;
    Helper::TextureFormat dataFormat;
    Helper::TextureType dataType;
    int levelOfDetail;
    bool mipMap;

    TextureParameters()
        : mag(Helper::TextureFiltering::LINEAR),
        min(Helper::TextureFiltering::LINEAR),
        wrapS(Helper::TextureWrap::REPEAT),
        wrapT(Helper::TextureWrap::REPEAT),
        internalFormat(Helper::TextureFormat::RGBA),
        dataFormat(Helper::TextureFormat::RGBA),
        dataType(Helper::TextureType::UNSIGNED_BYTE),
        levelOfDetail(0),
        mipMap(false)
    {
    }
};

class Texture2D {
    GLuint id;

    friend class Framebuffer;

public:
    ///
    /// @brief Constructor
    /// @param width Texture width
    /// @param height Texture height
    /// @param data Data ptr
    /// @param params Texture parameters
    ///
    explicit Texture2D(int width, int height, const void* data, const TextureParameters& params) : id(0) {
        glCheckError(glGenTextures(1, &id));
        glCheckError(glBindTexture(GL_TEXTURE_2D, id));
        glCheckError(glTexImage2D(GL_TEXTURE_2D, params.levelOfDetail,
            (GLenum)params.internalFormat, width, height, 0, (GLenum)params.dataFormat,
            (GLenum)params.dataType, data));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)params.min));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)params.mag));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)params.wrapT));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)params.wrapS));
        if (params.mipMap) {
            glCheckError(glGenerateMipmap(GL_TEXTURE_2D));
        }
        glCheckError(glBindTexture(GL_TEXTURE_2D, id));
    }

    ///
    /// @brief Destructor
    /// 
    ~Texture2D() { 
        glCheckError(glDeleteTextures(1, &id)); 
    }

    ///
    /// @brief Use
    /// 
    inline void use(unsigned char slot = 0) const {
        glCheckError(glActiveTexture(GL_TEXTURE0 + slot));
        glCheckError(glBindTexture(GL_TEXTURE_2D, id));
    }
};