#pragma once

#include "helper.hpp"

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
    inline void use(Helper::BufferMode mode) const {
        glCheckError(glBindBuffer((GLenum)mode, id));
    }

    ///
    /// @brief Set buffer data
    /// @param mode Buffer target mode
    /// @param size Buffer size
    /// @param data Data
    /// @param target Buffer data target mode
    ///
    inline void set(Helper::BufferMode mode, size_t size, const void* data, Helper::BufferTarget target) {
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
    inline void attribArray(GLuint index, int size, Helper::AttribType type, size_t stride, const void* ptr)
    {
        glCheckError(glEnableVertexAttribArray(index));
        glCheckError(glVertexAttribPointer(index, size, (GLenum)type, GL_FALSE, (GLsizei)stride, ptr));
    }
};