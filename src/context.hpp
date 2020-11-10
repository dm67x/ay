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
    /// @brief Draw arrays
    /// @param mode Draw mode
    /// @param first Specifies the starting index in the enabled arrays
    /// @param count Specifies the number of indices to be rendered
    ///
    void drawArrays(OpenGL::DrawMode mode, int first, size_t count) const;
};