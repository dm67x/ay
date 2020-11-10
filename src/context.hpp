#pragma once

#include "platform/opengl.hpp"
#include <map>

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
    PlatformId newShaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment);

    ///
    /// @brief Use shader
    /// @param name shader name
    ///
    void useShader(const std::string& name);
};