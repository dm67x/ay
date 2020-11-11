#pragma once

#include "platform/opengl.hpp"
#include <map>

struct Vec3;

class Context {
    std::map<std::string, PlatformId> shaders;
    PlatformId currentShader;

public:
    ///
    /// @brief Constructor of context
    ///
    Context() : shaders(), currentShader(0) {}

    ///
    /// @brief Destructor of context
    ///
    ~Context();

    ///
    /// @brief Clear screen
    /// @param r Red
    /// @param g Green
    /// @param b Blue
    /// @param a Alpha
    /// 
    void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) const;

    ///
    /// @brief Create shader from memory
    /// @param name shader name
    /// @param vertex vertex shader
    /// @param fragment fragment shader
    /// @return shader program id
    PlatformId shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment);

    ///
    /// @brief Create shader from file
    /// @param name shader name
    /// @param vertex vertex shader
    /// @param fragment fragment shader
    /// @return shader program id
    PlatformId shaderFromFile(const std::string& name, const std::string& vertex, const std::string& fragment);

    ///
    /// @brief Use shader
    /// @param name shader name
    ///
    void shaderUse(const std::string& name);

    ///
    /// @brief Shader uniform1f
    /// @param name uniform name
    /// @param value uniform value
    ///
    void shaderUniform(const std::string& name, float value) const;

    ///
    /// @brief Shader uniform3fv
    /// @param name uniform name
    /// @param value uniform value
    ///
    void shaderUniform(const std::string& name, const Vec3& value) const;

    ///
    /// @brief Create a new vao
    ///
    PlatformId vaoNew() const;

    ///
    /// @brief Destroy a vao
    /// @param id vao id
    ///
    void vaoDestroy(PlatformId id) const;

    ///
    /// @brief Bind vao
    /// @param id vao id
    ///
    void vaoBind(PlatformId id) const;

    ///
    /// @brief Create a new buffer
    /// @return buffer id
    ///
    PlatformId bufferNew() const;

    ///
    /// @brief Destroy the buffer specified by id
    /// @param id buffer to destroy
    ///
    void bufferDestroy(PlatformId id) const;

    ///
    /// @brief Bind buffer
    /// @param id buffer id
    /// @param mode buffer target mode
    ///
    void bufferBind(PlatformId id, OpenGL::BufferMode mode) const;

    ///
    /// @brief Set buffer data
    /// @param mode buffer target mode
    /// @param size buffer size
    /// @param data data
    /// @param target buffer data target mode
    ///
    void bufferData(OpenGL::BufferMode mode, size_t size, const void* data, OpenGL::BufferTarget target) const;

    ///
    /// @brief VertexAttribArray
    /// @param id Specifies the index of the generic vertex attribute to be modified
    /// @param size Specifies the number of components per generic vertex attribute
    /// @param type Specifies the data type of each component in the array
    /// @param stride Specifies the byte offset between consecutive generic vertex attributes
    /// @param ptr Specifies a offset of the first component of the first generic vertex attribute
    ///
    void bufferAttribArray(PlatformId id, int size, OpenGL::AttribType type, size_t stride, const void* ptr);    

    ///
    /// @brief Draw arrays
    /// @param mode Draw mode
    /// @param first Specifies the starting index in the enabled arrays
    /// @param count Specifies the number of indices to be rendered
    ///
    void drawArrays(OpenGL::DrawMode mode, int first, size_t count) const;

    ///
    /// @brief Draw elements call
    /// @param mode Specifies what kind of primitives to render
    /// @param count Specifies the number of elements to be rendered
    /// @param type Specifies the type of the values in indices
    /// @param indices Specifies a pointer to the location where the indices are stored
    ///
    void drawElements(OpenGL::DrawMode mode, size_t count, OpenGL::AttribType type, const void* indices);
};