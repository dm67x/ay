#pragma once

#include <glad.h>
#include <iostream>

#ifdef NDEBUG
#define glCheckError(expr) expr
#else
#define glCheckError(expr) {\
    expr;\
    OpenGLHelper::Error err = OpenGLHelper::getError();\
    if (err != OpenGLHelper::Error::NO_ERROR) {\
        std::cerr << "OpenGL error at line: " << __LINE__ << std::endl;\
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

    enum class DrawMode {
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
        POINTS = GL_POINTS,
        LINE_STRIP = GL_LINE_STRIP,
        LINE_LOOP = GL_LINE_LOOP,
        LINES = GL_LINES,
        LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
        LINES_ADJACENCY = GL_LINES_ADJACENCY,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        TRIANGLE_FAN = GL_TRIANGLE_FAN,
        PATCHES = GL_PATCHES
    };

    enum class BufferMode {
        ARRAY = GL_ARRAY_BUFFER,
        ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER
    };

    enum class BufferTarget {
        STATIC_DRAW = GL_STATIC_DRAW,
        STATIC_COPY = GL_STATIC_COPY,
        STATIC_READ = GL_STATIC_READ,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
        DYNAMIC_COPY = GL_DYNAMIC_COPY,
        DYNAMIC_READ = GL_DYNAMIC_READ,
    };

    enum class AttribType {
        FLOAT = GL_FLOAT,
        UNSIGNED_INT = GL_UNSIGNED_INT,
        BYTE = GL_BYTE,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        INT = GL_INT
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