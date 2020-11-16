#pragma once

#include "platform/platform.hpp"
#include "platform/opengl.hpp"
#include "math.hpp"
#include <map>
#include <array>

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
    inline void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) const {
        platform->clear(r, g, b, a);
    }

    ///
    /// @brief Set viewport
    /// @param x Left
    /// @param y Top
    /// @param w Width
    /// @param h Height
    /// 
    inline void viewport(int x, int y, int w, int h) const {
        platform->viewport(x, y, w, h);
    }

    ///
    /// @brief Create shader from memory
    /// @param name shader name
    /// @param vertex vertex shader
    /// @param fragment fragment shader
    /// @return ShaderProgram instance
    inline ShaderProgram* shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment) {
        auto program = platform->newProgram(vertex, fragment);
        shaders.insert(std::make_pair(name, program));
        return program;
    }

    ///
    /// @brief Create shader from file
    /// @param name shader name
    /// @param vertex vertex shader
    /// @param fragment fragment shader
    /// @return ShaderProgram instance
    ShaderProgram* shaderFromFile(const std::string& name, const std::string& vertex, const std::string& fragment);

    /// 
    /// @brief Destroy the shader
    /// @param name Shader name
    /// 
    inline void shaderDestroy(const std::string& name) {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            delete it->second;
            shaders.erase(it);
        }
    }

    ///
    /// @brief Use shader
    /// @param name Shader name
    ///
    inline void shaderUse(const std::string& name) {
        auto shader = shaders.find(name);
        if (shader != shaders.end()) {
            currentShader = shader->second;
        }
        currentShader->use();
    }

    ///
    /// @brief Shader uniform1f
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, float value) const {
        currentShader->uniform1f(name, value);
    }

    ///
    /// @brief Shader uniform1i
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, int value) const {
        currentShader->uniform1i(name, value);
    }

    ///
    /// @brief Shader uniform3fv
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, const Vec3& value) const {
        currentShader->uniform3f(name, new float[3]{ value.x, value.y, value.z });
    }

    ///
    /// @brief Shader uniform4fv
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, const Vec4& value) const {
        currentShader->uniform4f(name, new float[4]{ value.x, value.y, value.z, value.w });
    }

    ///
    /// @brief Shader uniformMatrix4fv
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, const Mat4& value) const {
        float values[16];
        std::memcpy(values, value.r1, sizeof(value.r1));
        std::memcpy(values + 4, value.r2, sizeof(value.r2));
        std::memcpy(values + 8, value.r3, sizeof(value.r3));
        std::memcpy(values + 12, value.r4, sizeof(value.r4));
        currentShader->uniformMatrix(name, &values[0], true);
    }

    ///
    /// @brief Create a new vao
    /// @return VertexArrayObject instance
    ///
    inline VertexArrayObject* vertexArrayObjectNew() const {
        return platform->newVertexArrayObject();
    }

    ///
    /// @brief Create a new buffer
    /// @return Buffer instance
    ///
    inline Buffer* bufferNew() const {
        return platform->newBuffer();
    }

    /// 
    /// @brief Create a new empty texture
    /// @param name Texture name
    /// @param width Texture width
    /// @param height Texture height
    /// @return Texture2D instance
    /// 
    inline Texture2D* texture2DNew(const std::string& name, int width, int height) {
        auto tex = platform->newTexture2D(width, height, nullptr, TextureParameters());
        textures.insert(std::make_pair(name, tex));
        return tex;
    }

    /// 
    /// @brief Create a new texture from file
    /// @param name Texture name
    /// @param filename Filename
    /// @return Texture2D instance
    /// 
    Texture2D* texture2DNew(const std::string& name, const std::string& filename);

    ///
    /// @brief Destroy the texture
    /// @param name Texture name
    /// 
    inline void texture2DDestroy(const std::string& name) {
        auto it = textures.find(name);
        if (it != textures.end()) {
            delete it->second;
            textures.erase(it);
        }
    }

    /// 
    /// @brief Get the texture by name
    /// @param name Texture name
    /// @return Texture id
    /// 
    inline Texture2D* texture2DGet(const std::string& name) const {
        auto it = textures.find(name);
        if (it != textures.end()) {
            return it->second;
        }
        return nullptr;
    }

    /// 
    /// @brief Create a new renderbuffer
    /// @param name Renderbuffer name
    /// @param width Width
    /// @param height Height
    /// @return Renderbuffer instance
    /// 
    inline Renderbuffer* renderbufferNew(const std::string& name, int width, int height) {
        auto rb = platform->newRenderbuffer(width, height);
        renderbuffers.insert(std::make_pair(name, rb));
        return rb;
    }

    /// 
    /// @brief Destroy the renderbuffer
    /// @param name Renderbuffer name
    /// 
    inline void renderbufferDestroy(const std::string& name) {
        auto it = renderbuffers.find(name);
        if (it != renderbuffers.end()) {
            delete it->second;
            renderbuffers.erase(it);
        }
    }

    /// 
    /// @brief Get the renderbuffer by name
    /// @param name Renderbuffer name
    /// @return Renderbuffer id
    /// 
    inline Renderbuffer* renderbufferGet(const std::string& name) const {
        auto it = renderbuffers.find(name);
        if (it != renderbuffers.end()) {
            return it->second;
        }
        return nullptr;
    }

    /// 
    /// @brief Create a new framebuffer
    /// @param name Framebuffer name
    /// @param params Framebuffer parameters
    /// @return Framebuffer instance
    /// 
    Framebuffer* framebufferNew(const std::string& name, const std::array<Texture2D*, 32>& colorAttachments, Renderbuffer* depthStencilAttachment = nullptr);

    /// 
    /// @brief Destroy the framebuffer
    /// @param name Framebuffer name
    /// 
    inline void framebufferDestroy(const std::string& name) {
        auto it = framebuffers.find(name);
        if (it != framebuffers.end()) {
            delete it->second;
            framebuffers.erase(it);
        }
    }

    /// 
    /// @brief Get the framebuffer
    /// @param name Framebuffer name
    ///
    inline Framebuffer* framebufferGet(const std::string& name) const {
        auto it = framebuffers.find(name);
        if (it != framebuffers.end()) {
            return it->second;
        }
        return nullptr;
    }
};