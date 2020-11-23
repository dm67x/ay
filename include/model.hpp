#pragma once

#include "types.hpp"
#include "transform.hpp"
#include <map>
#include <string>

namespace ay
{
    class Context;
    class Material;
    class ModelNode;

    class Model {
    public:
        ///
        /// @brief Destructor
        ///
        ~Model();

        ///
        /// @brief Create a new plane mesh
        /// @param ctx Context
        /// @return Plane mesh
        ///
        static Model* plane(Context* ctx);

        ///
        /// @brief Create a new cube mesh
        /// @param ctx Context
        /// @return Cube mesh
        ///
        static Model* cube(Context* ctx);

        ///
        /// @brief Create a new mesh from file (obj)
        /// @param ctx Context
        /// @param filename Filename
        /// @return Mesh
        ///
        static Model* fromFile(Context* ctx, const std::string& filename);

        ///
        /// @brief Render called each frame
        /// @param deltaTime Elapsed time between each frame
        ///
        void render(f32 deltaTime);

    private:
        ///
        /// @brief Constructor
        /// @param ctx Context
        ///
        Model(Context* ctx);

    private:
        friend class ModelNode;

    private:
        Context* ctx;
        ModelNode* root;
        std::map<i32, Material*> materials;

    public:
        Transform transform;
    };
}