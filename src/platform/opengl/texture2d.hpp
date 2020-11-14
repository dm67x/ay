#pragma once

#include "helper.hpp"
#include <utility>

enum class TextureWrap {
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
};

enum class TextureFiltering {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR
};

enum class TextureType {
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    BYTE = GL_BYTE,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    SHORT = GL_SHORT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    INT = GL_INT,
    HALF_FLOAT = GL_HALF_FLOAT,
    FLOAT = GL_FLOAT
};

enum class TextureFormat {
    RED = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    RED_INTEGER = GL_RED_INTEGER,
    RG_INTEGER = GL_RG_INTEGER,
    RGB_INTEGER = GL_RGB_INTEGER,
    RGBA_INTEGER = GL_RGBA_INTEGER,
    STENCIL_INDEX = GL_STENCIL_INDEX,
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL
};

struct TextureParameters {
    using Helper = OpenGLHelper;

    TextureFiltering mag;
    TextureFiltering min;
    TextureWrap wrapS;
    TextureWrap wrapT;
    TextureFormat internalFormat;
    TextureFormat dataFormat;
    TextureType dataType;
    int levelOfDetail;
    bool mipMap;

    TextureParameters()
        : mag(TextureFiltering::LINEAR),
        min(TextureFiltering::LINEAR),
        wrapS(TextureWrap::REPEAT),
        wrapT(TextureWrap::REPEAT),
        internalFormat(TextureFormat::RGBA),
        dataFormat(TextureFormat::RGBA),
        dataType(TextureType::UNSIGNED_BYTE),
        levelOfDetail(0),
        mipMap(false)
    {
    }
};

class Texture2D {
    GLuint id;
    int width;
    int height;

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

        this->width = width;
        this->height = height;
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

    ///
    /// @brief Reset
    ///
    inline static void reset() {
        glCheckError(glBindTexture(GL_TEXTURE_2D, 0));
    }

    ///
    /// @brief Get texture size
    /// 
    inline std::pair<int, int> getSize() const {
        return std::make_pair(width, height);
    }
};