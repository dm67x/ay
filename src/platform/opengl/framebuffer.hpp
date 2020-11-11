#pragma once

#include "helper.hpp"
#include <cstring>

struct FramebufferParameters {
    const Texture2D* colorAttachments[32];
    const Renderbuffer* depthStencilAttachment;

    FramebufferParameters() {
        std::memset(colorAttachments, 0, sizeof(colorAttachments));
        depthStencilAttachment = nullptr;
    }
};

class Framebuffer {
    GLuint id;

public:
    ///
    /// @brief Constructor
    /// @param params FramebufferParameters
    /// 
    explicit Framebuffer(const FramebufferParameters& params) : id(0) {
        glCheckError(glGenFramebuffers(1, &id));
        glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, id));
        for (int i = 0; i < 32; i++) {
            if (params.colorAttachments[i] != nullptr) {
                glCheckError(glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    (GLenum)(GL_COLOR_ATTACHMENT0 + i),
                    GL_TEXTURE_2D,
                    params.colorAttachments[i]->id, 0));
            }
        }

        if (params.depthStencilAttachment) {
            glCheckError(glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER,
                params.depthStencilAttachment->id));
        }

        GLenum status;
        glCheckError(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "The framebuffer is not complete" << std::endl;
            return;
        }

        glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    ///
    /// @brief Destructor
    /// 
    ~Framebuffer() { 
        glCheckError(glDeleteFramebuffers(1, &id)); 
    }
    
    ///
    /// @brief Use
    /// 
    inline void use() const {
        glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, id));
    }

    ///
    /// @brief Reset
    /// 
    inline static void reset() {
        glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
};