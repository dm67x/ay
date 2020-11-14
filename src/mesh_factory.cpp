#include "mesh_factory.hpp"
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"
#include <spdlog/spdlog.h>
#include <algorithm>

Mesh* MeshFactory::plane(Context* ctx) {
    Mesh* plane = new Mesh(ctx);
    plane->vertices = {
        Vertex(Vec3(-1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 0.f),
        Vertex(Vec3(1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 0.f),
        Vertex(Vec3(1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 1.f),
        Vertex(Vec3(-1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 1.f)
    };
    plane->indices = { 0, 2, 1, 0, 3, 2 };
    plane->build();
    return plane;
}

Mesh* MeshFactory::cube(Context* ctx) {
    Mesh* cube = new Mesh(ctx);
    cube->vertices = {
        Vertex(Vec3(-1.f, 1.f, -1.f), Vec3(), 0.25f, 0.f), // v1
        Vertex(Vec3(-1.f, 1.f, 1.f), Vec3(), 0.75f, 0.f), // v2
        Vertex(Vec3(1.f, 1.f, 1.f), Vec3(), 0.75f, 0.25f), // v3
        Vertex(Vec3(1.f, 1.f, -1.f), Vec3(), 0.25f, 0.25f), // v4
        Vertex(Vec3(-1.f, -1.f, 1.f), Vec3(), 0.75f, 0.75f), // v5
        Vertex(Vec3(-1.f, -1.f, -1.f), Vec3(), 0.25f, 0.75f), // v6
        Vertex(Vec3(1.f, -1.f, -1.f), Vec3(), 0.25f, 0.5f), // v7
        Vertex(Vec3(1.f, -1.f, 1.f), Vec3(), 0.75f, 0.5f), // v8
        
        Vertex(Vec3(-1.f, 1.f, -1.f), Vec3(), 0.f, 0.25f), // v12
        Vertex(Vec3(-1.f, 1.f, -1.f), Vec3(), 0.25f, 1.f), // v13
        Vertex(Vec3(-1.f, 1.f, 1.f), Vec3(), 1.f, 0.25f), // v22
        Vertex(Vec3(-1.f, 1.f, 1.f), Vec3(), 0.75f, 1.f), // v23
        Vertex(Vec3(-1.f, -1.f, 1.f), Vec3(), 1.f, 0.5f), // v52
        Vertex(Vec3(-1.f, -1.f, -1.f), Vec3(), 0.f, 0.5f) // v62
    };
    cube->indices = {
        0, 2, 1, 0, 3, 2,
        2, 3, 6, 2, 6, 7,
        7, 6, 5, 7, 5, 4,
        3, 8, 13, 3, 13, 6,
        10, 2, 7, 10, 7, 12,
        4, 5, 9, 4, 9, 11
    };
    cube->computeNormals();
    cube->build();
    return cube;
}

Mesh* MeshFactory::fromFile(Context* ctx, const std::string& filename) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;
    Mesh* parent = new Mesh(ctx);

    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    if (!warn.empty()) {
        spdlog::warn("{}", warn);
    }

    if (!err.empty()) {
        spdlog::error("{}", err);
    }

    if (!ret) {
        spdlog::error("Failed to parse {}", filename);
        return parent;
    }

    spdlog::info("Mesh {} loaded successfully", filename);
    return parent;
}