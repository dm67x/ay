#include "mesh.hpp"
#include "context.hpp"
#include <spdlog/spdlog.h>
#include "tiny_gltf.h"

Mesh::Mesh(Context* ctx) 
    : Object(ctx), 
    vao(nullptr), 
    ebo(nullptr), 
    vbos(), 
    drawMode(DrawMode::TRIANGLES),
    drawType(DrawType::UNSIGNED_INT),
    indicesCount(0)
{
    vao = ctx->vertexArrayObjectNew();
    ebo = ctx->bufferNew();
}

Mesh::~Mesh() {
    delete vao;
    delete ebo;

    for (auto vbo : vbos) {
        delete vbo.second;
    }

    for (auto mesh : children) {
        delete mesh;
    }
}

Mesh* Mesh::plane(Context* ctx) {
    Mesh* plane = new Mesh(ctx);
    std::vector<Vertex> vertices = {
        Vertex(Vec3(-1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 0.f),
        Vertex(Vec3(1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 0.f),
        Vertex(Vec3(1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 1.f),
        Vertex(Vec3(-1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 1.f)
    };
    std::vector<unsigned int> indices = { 0, 2, 1, 0, 3, 2 };

    // Build
    plane->vao->use();
    plane->vbos.insert(std::make_pair(0, ctx->bufferNew()));
    auto vbo = plane->vbos[0];
    vbo->use(BufferMode::ARRAY);
    vbo->set(BufferMode::ARRAY, sizeof(Vertex) * vertices.size(), vertices.data(), BufferTarget::STATIC_DRAW);
    vbo->attribArray(0, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    vbo->attribArray(1, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    vbo->attribArray(2, 2, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, u));
    plane->ebo->use(BufferMode::ELEMENT_ARRAY);
    plane->ebo->set(BufferMode::ELEMENT_ARRAY, sizeof(unsigned int) * indices.size(), indices.data(), BufferTarget::STATIC_DRAW);
    Buffer::reset(BufferMode::ELEMENT_ARRAY);
    VertexArrayObject::reset();

    plane->indicesCount = indices.size();
    return plane;
}

Mesh* Mesh::cube(Context* ctx) {
    Mesh* cube = new Mesh(ctx);
    std::vector<Vertex> vertices = {
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
    std::vector<unsigned int> indices = {
        0, 2, 1, 0, 3, 2,
        2, 3, 6, 2, 6, 7,
        7, 6, 5, 7, 5, 4,
        3, 8, 13, 3, 13, 6,
        10, 2, 7, 10, 7, 12,
        4, 5, 9, 4, 9, 11
    };
    cube->computeNormals(vertices, indices, vertices);

    // Build
    cube->vao->use();
    cube->vbos.insert(std::make_pair(0, ctx->bufferNew()));
    auto vbo = cube->vbos[0];
    vbo->use(BufferMode::ARRAY);
    vbo->set(BufferMode::ARRAY, sizeof(Vertex) * vertices.size(), vertices.data(), BufferTarget::STATIC_DRAW);
    vbo->attribArray(0, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    vbo->attribArray(1, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    vbo->attribArray(2, 2, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, u));
    cube->ebo->use(BufferMode::ELEMENT_ARRAY);
    cube->ebo->set(BufferMode::ELEMENT_ARRAY, sizeof(unsigned int) * indices.size(), indices.data(), BufferTarget::STATIC_DRAW);
    Buffer::reset(BufferMode::ELEMENT_ARRAY);
    VertexArrayObject::reset();

    cube->indicesCount = indices.size();
    return cube;
}

Mesh* Mesh::fromFile(Context* ctx, const std::string& filename) {
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

    // Get only geometry for now
    auto scene = model.scenes[model.defaultScene];
    for (auto nodeIndex : scene.nodes) {
        auto node = model.nodes[nodeIndex];
        auto mesh = model.meshes[node.mesh];
        for (auto primitive : mesh.primitives) {
            Mesh* finalMesh = new Mesh(ctx);
            finalMesh->vao->use();

            // VBOs
            for (auto& attribute : primitive.attributes) {
                auto accessor = model.accessors[attribute.second];
                auto bufferView = model.bufferViews[accessor.bufferView];
                auto buffer = model.buffers[bufferView.buffer];
                int stride = accessor.ByteStride(bufferView);

                if (bufferView.target != (int)BufferMode::ARRAY) {
                    continue;
                }

                int index = -1;
                if (attribute.first == "POSITION") {
                    index = 0;
                }
                else if (attribute.first == "NORMAL") {
                    index = 1;
                }
                else if (attribute.first == "TEXCOORD_0") {
                    index = 2;
                }

                if (index >= 0) {
                    auto it = finalMesh->vbos.find(index);
                    if (it == finalMesh->vbos.end()) {
                        finalMesh->vbos.insert(std::make_pair(index, ctx->bufferNew()));
                        it = finalMesh->vbos.find(index);
                        it->second->use(BufferMode::ARRAY);
                        it->second->set(BufferMode::ARRAY, bufferView.byteLength, buffer.data.data() + bufferView.byteOffset, BufferTarget::STATIC_DRAW);
                    }

                    it->second->attribArray(
                        (unsigned int)index,
                        accessor.type,
                        (AttribType)accessor.componentType,
                        (size_t)stride,
                        (void*)accessor.byteOffset);
                }
            }

            // EBO (indices)
            finalMesh->ebo->use(BufferMode::ELEMENT_ARRAY);
            auto accessor = model.accessors[primitive.indices];
            auto indicesBufferView = model.bufferViews[accessor.bufferView];
            auto indicesBuffer = model.buffers[indicesBufferView.buffer];
            finalMesh->ebo->set(BufferMode::ELEMENT_ARRAY, indicesBufferView.byteLength, indicesBuffer.data.data() + indicesBufferView.byteOffset, BufferTarget::STATIC_DRAW);
            Buffer::reset(BufferMode::ELEMENT_ARRAY);
            VertexArrayObject::reset();

            finalMesh->drawMode = (DrawMode)primitive.mode;
            finalMesh->indicesCount = accessor.count;
            finalMesh->drawType = (DrawType)accessor.componentType;
            parent->addChild(finalMesh);
        }
    }

    spdlog::info("Mesh {} loaded successfully", filename);
    return parent;
}

void Mesh::render(float deltaTime) {
    (void)deltaTime;

    Mat4 _transform = transform.getTransform();
    if (parent != nullptr) {
        _transform = _transform * parent->transform.getTransform();
    }

    ctx->shaderUniform("modelMatrix", _transform);
    ctx->shaderUniform("normalMatrix", _transform.inverse().transpose());
    vao->use();
    ebo->use(BufferMode::ELEMENT_ARRAY);
    vao->drawElements(drawMode, indicesCount, drawType, nullptr);
    Buffer::reset(BufferMode::ELEMENT_ARRAY);
    VertexArrayObject::reset();

    for (auto mesh : children) {
        mesh->render(deltaTime);
    }
}

void Mesh::computeNormals(
    const std::vector<Vertex>& vertices, 
    const std::vector<unsigned int>& indices, 
    std::vector<Vertex>& output) 
{
    output = vertices;

    // Get faces
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int v1i = indices[i];
        unsigned int v2i = indices[i + 1];
        unsigned int v3i = indices[i + 2];

        Vec3 v1 = vertices[v1i].position;
        Vec3 v2 = vertices[v2i].position;
        Vec3 v3 = vertices[v3i].position;

        Vec3 v1v2 = v2 - v1;
        Vec3 v1v3 = v3 - v1;

        Vec3 normal = (v1v2 * v1v3).normalize();

        output[v1i].normal = normal;
        output[v2i].normal = normal;
        output[v3i].normal = normal;
    }
}