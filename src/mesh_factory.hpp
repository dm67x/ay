#pragma once

#include "mesh.hpp"
#include <string>

struct MeshFactory {
    ///
    /// @brief Create a new plane mesh
    /// @param ctx Context
    /// @return Plane mesh
    ///
    static Mesh* plane(Context* ctx);

    ///
    /// @brief Create a new mesh from file (obj)
    /// @param ctx Context
    /// @param filename Filename
    /// @return Mesh
    ///
    static Mesh* fromFile(Context* ctx, const std::string& filename);
};