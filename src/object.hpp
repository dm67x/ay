#pragma once

#include <vector>
#include <algorithm>
#include "transform.hpp"

class Context;

class Object {
protected:
    Context* ctx;
    Object* parent;
    std::vector<Object*> children;

public:
    Transform transform;

public:
    ///
    /// @brief Constructor
    /// @param ctx Context pointer
    ///
    Object(Context* ctx, Object* parent = nullptr) 
        : ctx(ctx), parent(parent), children() 
    {
        start();
    }

    ///
    /// @brief Destructor
    ///
    virtual ~Object() {
        destroy();
    }

    ///
    /// @brief Add child
    /// @param obj Object to add to the children
    /// @return The added object
    ///
    inline Object* addChild(Object* obj) {
        auto it = std::find(children.begin(), children.end(), obj);
        if (it == children.end()) {
            if (obj->parent != nullptr) {
                obj->parent->removeChild(obj);
            }

            obj->parent = this;
            children.push_back(obj);
        }
        return obj;
    }

    ///
    /// @brief Remove child
    /// @param obj Object to remove from children
    ///
    inline void removeChild(Object* obj) {
        auto it = std::find(children.begin(), children.end(), obj);
        if (it != children.end()) {
            obj->parent = nullptr;
            children.erase(it);
        }
    }

    ///
    /// @brief Start called once after the creation of the object
    ///
    virtual void start() {};

    ///
    /// @brief Destroy called once before the destruction of the object
    ///
    virtual void destroy() {};

    ///
    /// @brief Update called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    virtual void update(float deltaTime) = 0;

    ///
    /// @brief Render called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    virtual void render(float deltaTime) = 0;
};