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
    }

    ///
    /// @brief Destructor
    ///
    virtual ~Object() {}

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
    /// @brief Render called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    virtual void render(float deltaTime) = 0;

    ///
    /// @brief Get model matrix
    /// @return Mat4
    ///
    virtual glm::mat4 getTransform() const {
        glm::mat4 t = transform.getTransform(); // local transform
        if (parent != nullptr) {
            t = t * parent->transform.getTransform();
        }
        return t;
    }
};