#pragma once

#include "helper.hpp"

class VertexArrayObject {
    using Helper = OpenGLHelper;

    GLuint id;

public:
    ///
    /// @brief Constructor
    ///
    VertexArrayObject() : id(0) {
        glCheckError(glGenVertexArrays(1, &id));
    }

    ///
    /// @brief Destructor
    ///
    ~VertexArrayObject() { 
        glCheckError(glDeleteVertexArrays(1, &id)); 
    }

    ///
    /// @brief Use
    ///
    inline void use() const {
        glCheckError(glBindVertexArray(id));
    }

    ///
    /// @brief Draw arrays call
    /// @param first Specifies the starting index in the enabled arrays
    /// @param count Specifies the number of indices to be rendered
    /// 
    inline void drawArrays(int first, size_t count) const {
        glCheckError(glDrawArrays(GL_TRIANGLES, first, (GLsizei)count));
    }

    ///
    /// @brief Draw elements call
    /// @param count Specifies the number of elements to be rendered
    /// @param type Specifies the type of the values in indices
    /// @param indices Specifies a pointer to the location where the indices are stored
    ///
    inline void drawElements(size_t count, Helper::AttribType type, const void* indices) {
        glCheckError(glDrawElements(GL_TRIANGLES, (GLsizei)count, (GLenum)type, indices));
    }
};