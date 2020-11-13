#pragma once

#include <glad.h>
#include <spdlog/spdlog.h>

#ifdef NDEBUG
#define glCheckError(expr) expr
#else
#define glCheckError(expr) {\
    expr;\
    OpenGLHelper::Error err = OpenGLHelper::getError();\
    if (err != OpenGLHelper::Error::NO_ERROR) {\
        spdlog::error("OpenGL error in {} at line: {}", __FILE__, __LINE__); \
    } }
#endif

class OpenGLHelper {
    friend class Context;
    friend class OpenGL;
    friend class ShaderProgram;
    friend class Texture2D;
    friend struct TextureParameters;
    friend class Renderbuffer;
    friend class Framebuffer;
    friend struct FramebufferParameters;
    friend class VertexArrayObject;
    friend class Buffer;

    enum class Error {
        NO_ERROR,
        INVALID_ENUM,
        INVALID_VALUE,
        INVALID_OPERATION,
        STACK_OVERFLOW,
        STACK_UNDERFLOW,
        OUT_OF_MEMORY,
        INVALID_FRAMEBUFFER_OPERATION
    };

    ///
    /// @brief Get OpenGL error
    /// @return Error code
    ///
    inline static Error getError() {
        switch (glGetError()) {
        case GL_INVALID_ENUM: return Error::INVALID_ENUM;
        case GL_INVALID_VALUE: return Error::INVALID_VALUE;
        case GL_INVALID_OPERATION: return Error::INVALID_OPERATION;
        case GL_STACK_OVERFLOW: return Error::STACK_OVERFLOW;
        case GL_STACK_UNDERFLOW: return Error::STACK_UNDERFLOW;
        case GL_OUT_OF_MEMORY: return Error::OUT_OF_MEMORY;
        case GL_INVALID_FRAMEBUFFER_OPERATION: return Error::INVALID_FRAMEBUFFER_OPERATION;
        default: return Error::NO_ERROR;
        }
    }
};