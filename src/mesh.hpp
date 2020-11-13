#pragma once

#include "object.hpp"

class Mesh : public Object {
public:
    ///
    /// @brief Constructor
    /// @param ctx Context pointer
    ///
    Mesh(Context* ctx) : Object(ctx) {}

    ///
    /// @brief Start called once after the creation of the object
    ///
    void start() override;

    ///
    /// @brief Destroy called once before the destruction of the object
    ///
    void destroy() override;

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