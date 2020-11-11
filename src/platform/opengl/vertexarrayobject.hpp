#pragma once

#include "helper.hpp"

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

enum class DrawType {
    FLOAT = GL_FLOAT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    BYTE = GL_BYTE,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    INT = GL_INT
};

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
    /// @param mode Draw mode
    /// @param first Specifies the starting index in the enabled arrays
    /// @param count Specifies the number of indices to be rendered
    /// 
    inline void drawArrays(DrawMode mode, int first, size_t count) const {
        glCheckError(glDrawArrays((GLenum)mode, first, (GLsizei)count));
    }

    ///
    /// @brief Draw elements call
    /// @param mode Draw mode
    /// @param count Specifies the number of elements to be rendered
    /// @param type Specifies the type of the values in indices
    /// @param indices Specifies a pointer to the location where the indices are stored
    ///
    inline void drawElements(DrawMode mode, size_t count, DrawType type, const void* indices) {
        glCheckError(glDrawElements((GLenum)mode, (GLsizei)count, (GLenum)type, indices));
    }
};