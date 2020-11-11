#pragma once

#include "helper.hpp"

class Renderbuffer {
    GLuint id;
    int width;
    int height;

    friend class Framebuffer;

public:
    ///
    /// @brief Constructor
    /// @param width Width
    /// @param height Height
    /// 
    explicit Renderbuffer(int width, int height) : id(0), width(width), height(height) {
        glCheckError(glGenRenderbuffers(1, &id));
        glCheckError(glBindRenderbuffer(GL_RENDERBUFFER, id));
        glCheckError(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
        glCheckError(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    }

    ///
    /// @brief Destructor
    /// 
    ~Renderbuffer() { 
        glCheckError(glDeleteRenderbuffers(1, &id)); 
    }
};