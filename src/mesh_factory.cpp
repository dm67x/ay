#include "mesh_factory.hpp"
#include "context.hpp"
#include <spdlog/spdlog.h>
#include "tiny_gltf.h"

Mesh* MeshFactory::plane(Context* ctx) {
    Mesh* plane = new Mesh(ctx);
    plane->vertices = {
        Vertex(Vec3(-1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 0.f),
        Vertex(Vec3(1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 0.f),
        Vertex(Vec3(1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 1.f),
        Vertex(Vec3(-1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 1.f)
    };
    plane->indices = { 0, 2, 1, 0, 3, 2 };
    
    // Build
    plane->vao->use();
    plane->vbo->use(BufferMode::ARRAY);
    plane->vbo->set(BufferMode::ARRAY, sizeof(Vertex) * plane->vertices.size(), plane->vertices.data(), BufferTarget::STATIC_DRAW);
    plane->vbo->attribArray(0, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    plane->vbo->attribArray(1, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    plane->vbo->attribArray(2, 2, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, u));
    plane->ebo->use(BufferMode::ELEMENT_ARRAY);
    plane->ebo->set(BufferMode::ELEMENT_ARRAY, sizeof(unsigned int) * plane->indices.size(), plane->indices.data(), BufferTarget::STATIC_DRAW);
    Buffer::reset(BufferMode::ELEMENT_ARRAY);
    VertexArrayObject::reset();

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
    
    // Build
    cube->vao->use();
    cube->vbo->use(BufferMode::ARRAY);
    cube->vbo->set(BufferMode::ARRAY, sizeof(Vertex) * cube->vertices.size(), cube->vertices.data(), BufferTarget::STATIC_DRAW);
    cube->vbo->attribArray(0, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    cube->vbo->attribArray(1, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    cube->vbo->attribArray(2, 2, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, u));
    cube->ebo->use(BufferMode::ELEMENT_ARRAY);
    cube->ebo->set(BufferMode::ELEMENT_ARRAY, sizeof(unsigned int) * cube->indices.size(), cube->indices.data(), BufferTarget::STATIC_DRAW);
    Buffer::reset(BufferMode::ELEMENT_ARRAY);
    VertexArrayObject::reset();

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