#pragma once

#include "helper.hpp"

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

class Buffer {
    using Helper = OpenGLHelper;

    GLuint id;

public:
    ///
    /// @brief Constructor
    ///
    Buffer() : id(0) {
        glCheckError(glGenBuffers(1, &id));
    }

    ///
    /// @brief Destructor
    ///
    ~Buffer() {
        glCheckError(glDeleteBuffers(1, &id));
    }

    ///
    /// @brief Use buffer
    /// @param mode Buffer target mode
    ///
    inline void use(BufferMode mode) const {
        glCheckError(glBindBuffer((GLenum)mode, id));
    }

    ///
    /// @brief Use buffer
    /// @param mode Buffer target mode
    ///
    inline static void reset(BufferMode mode) {
        glCheckError(glBindBuffer((GLenum)mode, 0));
    }

    ///
    /// @brief Set buffer data
    /// @param mode Buffer target mode
    /// @param size Buffer size
    /// @param data Data
    /// @param target Buffer data target mode
    ///
    inline void set(BufferMode mode, size_t size, const void* data, BufferTarget target) {
        glCheckError(glBufferData((GLenum)mode, (GLsizeiptr)size, data, (GLenum)target));
    }

    ///
    /// @brief VertexAttribArray
    /// @param index Specifies the index of the generic vertex attribute to be modified
    /// @param size Specifies the number of components per generic vertex attribute
    /// @param type Specifies the data type of each component in the array
    /// @param stride Specifies the byte offset between consecutive generic vertex attributes
    /// @param ptr Specifies a offset of the first component of the first generic vertex attribute
    ///
    inline void attribArray(GLuint index, int size, AttribType type, size_t stride, const void* ptr)
    {
        glCheckError(glEnableVertexAttribArray(index));
        glCheckError(glVertexAttribPointer(index, size, (GLenum)type, GL_FALSE, (GLsizei)stride, ptr));
    }
};