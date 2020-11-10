#pragma once

#include "platform/opengl.hpp"
#include <vector>

class Context {
    std::vector<PlatformId> shaders;

public:
    ///
    /// @brief Constructor of context
    ///
    Context() : shaders() {}

    ///
    /// @brief Destructor of context
    ///
    ~Context();
};