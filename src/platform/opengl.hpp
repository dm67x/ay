#pragma once

#include "platform.hpp"
#include "opengl/helper.hpp"
#include "opengl/shader.hpp"
#include "opengl/texture2d.hpp"
#include "opengl/renderbuffer.hpp"
#include "opengl/framebuffer.hpp"
#include "opengl/vertexarrayobject.hpp"
#include "opengl/buffer.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <sstream>
#include <cstdio>

class OpenGL : public Platform {
    friend class Context;

    ///
    /// @brief Load OpenGL Context
    /// @return True if context loaded false otherwise
    ///
    OpenGL() {
        if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
            spdlog::critical("cannot initialize glad");
            std::exit(EXIT_FAILURE);
        }
    }

    ///
    /// @brief Get OpenGL Version
    /// @return Version
    ///
    inline const std::string getVersion() override {
        std::stringstream version;
        version << glGetString(GL_VERSION);
        return version.str();
    }

    ///
    /// @brief Get OpenGL Vendor
    /// @return Vendor
    ///
    inline const std::string getVendor() override {
        std::stringstream version;
        version << glGetString(GL_VENDOR);
        return version.str();
    }

    ///
    /// @brief Clear screen
    /// @param r Red
    /// @param g Green
    /// @param b Blue
    /// @param a Alpha
    /// 
    inline void clear(float r, float g, float b, float a) override {
        glCheckError(glClearColor(r, g, b, a));
        glCheckError(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    ///
    /// @brief Set viewport
    /// @param x Left
    /// @param y Top
    /// @param w Width
    /// @param h Height
    /// 
    inline void viewport(int x, int y, int w, int h) override {
        glCheckError(glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h));
    }

    ///
    /// @brief Create a new shader program
    /// @param vertex vertex source
    /// @param fragment fragment source
    /// @return ShaderProgram object instance
    ///
    inline ShaderProgram* newProgram(const std::string& vertex, const std::string& fragment) override {
        return new ShaderProgram(vertex, fragment);
    }

    ///
    /// @brief Create a new texture
    /// @param width Texture width
    /// @param height Texture height
    /// @param data Data ptr
    /// @param params Texture parameters
    /// @return Texture2D object instance
    ///
    inline Texture2D* newTexture2D(int width, int height, const void* data, const TextureParameters& params) override {
        return new Texture2D(width, height, data, params);
    }

    ///
    /// @brief Create a new renderbuffer
    /// @param width Renderbuffer width
    /// @param height Renderbuffer height
    /// @return Renderbuffer object instance
    /// 
    inline Renderbuffer* newRenderbuffer(int width, int height) override {
        return new Renderbuffer(width, height);
    }

    ///
    /// @brief Create a new framebuffer
    /// @param params FramebufferParameters
    /// @return Framebuffer object instance
    /// 
    inline Framebuffer* newFramebuffer(const FramebufferParameters& params) {
        return new Framebuffer(params);
    }

    ///
    /// @brief Create a new vertex array object
    /// @return VertexArrayObject object instance
    ///
    inline VertexArrayObject* newVertexArrayObject() override {
        return new VertexArrayObject();
    }

    ///
    /// @brief Create buffer
    /// @return Buffer object instance
    ///
    inline Buffer* newBuffer() {
        return new Buffer();
    }
};