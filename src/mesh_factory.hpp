#pragma once

#include "mesh.hpp"

struct MeshFactory {
    ///
    /// @brief Create a new plane mesh
    /// @return Plane mesh
    ///
    static Mesh plane(Context* ctx) {
        Mesh plane(ctx);
        plane.vertices = {
            Vertex(Vec3(-1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 0.f),
            Vertex(Vec3(1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 0.f),
            Vertex(Vec3(1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 1.f),
            Vertex(Vec3(-1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 1.f)
        };
        plane.indices = { 0, 2, 1, 0, 3, 2 };
        plane.build();
        return plane;
    }
};