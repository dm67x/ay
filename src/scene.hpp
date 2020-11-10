#pragma once

class Context;

class Scene {
    const Context& ctx;

public:
    ///
    /// @brief Build a new scene
    /// @param ctx The context
    ///
    Scene(const Context& ctx) : ctx(ctx) {}
};