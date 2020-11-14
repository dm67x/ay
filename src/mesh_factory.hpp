#pragma once

#include "mesh.hpp"
#include <string>
#include <vector>
#include <future>

namespace tinyobj {
    struct attrib_t;
    struct shape_t;
}

struct MeshFactory {
    struct AsyncMesh {
        std::vector<Mesh::Vertex> vertices;
        std::vector<unsigned int> indices;
        bool computeNormals;
        std::vector<AsyncMesh*> children;

        AsyncMesh() : vertices(), indices(), computeNormals(false), children() {
        }

        ~AsyncMesh() {
            for (auto child : children) {
                delete child;
            }
        }

        inline Mesh* convertTo(Context* ctx) const {
            Mesh* parent = new Mesh(ctx);
            parent->vertices = vertices;
            parent->indices = indices;

            for (auto child : children) {
                parent->addChild(child->convertTo(ctx));
            }

            if (computeNormals) {
                parent->computeNormals();
            }
            parent->build();
            return parent;
        }
    };

    ///
    /// @brief Create a new plane mesh
    /// @param ctx Context
    /// @return Plane mesh
    ///
    static Mesh* plane(Context* ctx);

    ///
    /// @brief Create a new cube mesh
    /// @param ctx Context
    /// @return Cube mesh
    ///
    static Mesh* cube(Context* ctx);

    ///
    /// @brief Create a new mesh from file (obj)
    /// @param ctx Context
    /// @param filename Filename
    /// @return Mesh
    ///
    static std::future<AsyncMesh*> fromFile(const std::string& filename);

private:
    static bool computeMesh(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib, std::vector<Mesh::Vertex>* vertices, std::vector<unsigned int>* indices);
};