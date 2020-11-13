#pragma once

#include "mesh.hpp"

struct MeshFactory {
    ///
    /// @brief Create a new plane mesh
    /// @return Plane mesh
    ///
    static Mesh plane(Context* ctx) {
        Mesh plane(ctx);
        plane.vertices = {};
        plane.indices = {};
        plane.build();
        return plane;
    }
};