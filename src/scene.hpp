#pragma once

#include "object.hpp"

class Scene : public Object {
public:
    ///
    /// @brief Create a new scene
    /// @param ctx The context
    ///
    Scene(Context* ctx) : Object(ctx) {}

    ///
    /// @brief Destructor
    ///
    virtual ~Scene();

    ///
    /// @brief Update called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    void update(float deltaTime) override;

    ///
    /// @brief Render called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    void render(float deltaTime) override;
};