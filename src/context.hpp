#pragma once

#include "platform/platform.hpp"
#include "platform/opengl.hpp"
#include <map>
#include <array>

struct Vec3;
struct Mat4;

class Context {
    Platform* platform;
    std::map<std::string, ShaderProgram*> shaders;
    std::map<std::string, Texture2D*> textures;
    std::map<std::string, Renderbuffer*> renderbuffers;
    std::map<std::string, Framebuffer*> framebuffers;
    ShaderProgram* currentShader;

public:
    ///
    /// @brief Constructor of context
    ///
    Context() 
        : platform(new OpenGL()), 
        shaders(), 
        textures(), 
        renderbuffers(), 
        framebuffers(), 
        currentShader(nullptr) 
    {
    }

    ///
    /// @brief Destructor of context
    ///
    ~Context();

    ///
    /// @brief Get OpenGL Version
    /// @return Version
    ///
    inline const std::string getVersion() {
        return platform->getVersion();
    }

    ///
    /// @brief Get Vendor
    /// @return Vendor
    ///
    inline const std::string getVendor() {
        return platform->getVendor();
    }

    ///
    /// @brief Clear screen
    /// @param r Red
    /// @param g Green
    /// @param b Blue
    /// @param a Alpha
    /// 
    void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) const;

    ///
    /// @brief Set viewport
    /// @param x Left
    /// @param y Top
    /// @param w Width
    /// @param h Height
    /// 
    void viewport(int x, int y, int w, int h) const;

    ///
    /// @brief Create shader from memory
    /// @param name shader name
    /// @param vertex vertex shader
    /// @param fragment fragment shader
    /// @return shader program id
    void shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment);

    ///
    /// @brief Create shader from file
    /// @param name shader name
    /// @param vertex vertex shader
    /// @param fragment fragment shader
    /// @return shader program id
    void shaderFromFile(const std::string& name, const std::string& vertex, const std::string& fragment);

    /// 
    /// @brief Destroy the shader
    /// @param name Shader name
    /// 
    void shaderDestroy(const std::string& name);

    ///
    /// @brief Use shader
    /// @param name Shader name
    ///
    void shaderUse(const std::string& name);

    ///
    /// @brief Shader uniform1f
    /// @param name uniform name
    /// @param value uniform value
    ///
    void shaderUniform(const std::string& name, float value) const;

    ///
    /// @brief Shader uniform1i
    /// @param name uniform name
    /// @param value uniform value
    ///
    void shaderUniform(const std::string& name, int value) const;

    ///
    /// @brief Shader uniform3fv
    /// @param name uniform name
    /// @param value uniform value
    ///
    void shaderUniform(const std::string& name, const Vec3& value) const;

    ///
    /// @brief Shader uniformMatrix4fv
    /// @param name uniform name
    /// @param value uniform value
    ///
    void shaderUniform(const std::string& name, const Mat4& value) const;

    ///
    /// @brief Create a new vao
    /// @return VertexArrayObject instance
    ///
    VertexArrayObject* vertexArrayObjectNew() const;

    ///
    /// @brief Create a new buffer
    /// @return Buffer instance
    ///
    Buffer* bufferNew() const;

    /// 
    /// @brief Create a new empty texture
    /// @param name Texture name
    /// @param width Texture width
    /// @param height Texture height
    /// 
    void texture2DNew(const std::string& name, int width, int height);

    /// 
    /// @brief Create a new texture from file
    /// @param name Texture name
    /// @param filename Filename
    /// 
    void texture2DNew(const std::string& name, const std::string& filename);

    ///
    /// @brief Destroy the texture
    /// @param name Texture name
    /// 
    void texture2DDestroy(const std::string& name);

    /// 
    /// @brief Get the texture by name
    /// @param name Texture name
    /// @return Texture id
    /// 
    Texture2D* texture2DGet(const std::string& name) const;

    /// 
    /// @brief Create a new renderbuffer
    /// @param name Renderbuffer name
    /// @param width Width
    /// @param height Height
    /// 
    void renderbufferNew(const std::string& name, int width, int height);

    /// 
    /// @brief Destroy the renderbuffer
    /// @param name Renderbuffer name
    /// 
    void renderbufferDestroy(const std::string& name);

    /// 
    /// @brief Get the renderbuffer by name
    /// @param name Renderbuffer name
    /// @return Renderbuffer id
    /// 
    Renderbuffer* renderbufferGet(const std::string& name) const;

    /// 
    /// @brief Create a new framebuffer
    /// @param name Framebuffer name
    /// @param params Framebuffer parameters
    /// 
    void framebufferNew(const std::string& name, const std::array<Texture2D*, 32>& colorAttachments, Renderbuffer* depthStencilAttachment = nullptr);

    /// 
    /// @brief Destroy the framebuffer
    /// @param name Framebuffer name
    /// 
    void framebufferDestroy(const std::string& name);

    /// 
    /// @brief Get the framebuffer
    /// @param name Framebuffer name
    ///
    Framebuffer* framebufferGet(const std::string& name) const;
};