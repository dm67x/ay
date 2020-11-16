#include "mesh.hpp"
#include "context.hpp"
#include <spdlog/spdlog.h>
#include "tiny_gltf.h"

Mesh::Mesh(Context* ctx) 
    : Object(ctx), 
    vao(0), 
    ebo(0), 
    vbos(), 
    drawMode(GL_TRIANGLES),
    drawType(GL_UNSIGNED_INT),
    indicesCount(0),
    axisVao(0),
    axisBuffer(0),
    isDebugMode(false),
    cloneOf(nullptr)
{
    vao = ctx->vaoNew();
    ebo = ctx->bufferNew();

    // create axis
    axisVao = ctx->vaoNew();
    axisBuffer = ctx->bufferNew();

    float axis[] = {
        0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
        1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
        0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 0.f, 0.f, 1.f,
        0.f, 0.f, 1.f, 0.f, 0.f, 1.f
    };

    ctx->vaoUse(axisVao);
    ctx->bufferUse<BufferUsage::ARRAY>(axisBuffer);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(axis), &axis);
    ctx->bufferAttribute(0, GL_FLOAT, 3, 6 * sizeof(float), nullptr);
    ctx->bufferAttribute(3, GL_FLOAT, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    ctx->bufferUse<BufferUsage::ARRAY>(0);
    ctx->vaoUse(0);
}

Mesh::~Mesh() {
    if (cloneOf != nullptr) {
        ctx->vaoDispose(vao);
        ctx->bufferDispose(ebo);
        ctx->bufferDispose(axisBuffer);

        for (auto vbo : vbos) {
            ctx->bufferDispose(vbo.second);
        }
    }

    for (auto mesh : children) {
        delete mesh;
    }
}

Mesh* Mesh::clone() const {
    Mesh* mesh = new Mesh(ctx);
    ctx->vaoDispose(mesh->vao);
    ctx->bufferDispose(mesh->ebo);

    mesh->vao = vao;
    mesh->ebo = ebo;
    mesh->vbos = vbos;
    mesh->drawMode = drawMode;
    mesh->drawType = drawType;
    mesh->cloneOf = this;
    mesh->indicesCount = indicesCount;

    for (auto child : children) {
        auto childMesh = static_cast<Mesh*>(child);
        Mesh* cloneChildMesh = childMesh->clone();
        mesh->addChild(cloneChildMesh);
    }
    return mesh;
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
    ctx->vaoUse(plane->vao);
    Buffer vbo = ctx->bufferNew();
    plane->vbos.insert(std::make_pair(0, vbo));
    ctx->bufferUse<BufferUsage::ARRAY>(vbo);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>((GLsizeiptr)(sizeof(Vertex) * vertices.size()), vertices.data());
    ctx->bufferAttribute(0, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    ctx->bufferAttribute(1, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    ctx->bufferAttribute(2, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, u));
    ctx->bufferUse<BufferUsage::ELEMENT>(plane->ebo);
    ctx->bufferData<BufferUsage::ELEMENT, BufferTarget::STATIC_DRAW>((GLsizeiptr)(sizeof(unsigned int) * indices.size()), indices.data());
    ctx->bufferUse<BufferUsage::ELEMENT>(0);
    ctx->vaoUse(0);

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
    ctx->vaoUse(cube->vao);
    Buffer vbo = ctx->bufferNew();
    cube->vbos.insert(std::make_pair(0, vbo));
    ctx->bufferUse<BufferUsage::ARRAY>(vbo);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>((GLsizeiptr)(sizeof(Vertex) * vertices.size()), vertices.data());
    ctx->bufferAttribute(0, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    ctx->bufferAttribute(1, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    ctx->bufferAttribute(2, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, u));
    ctx->bufferUse<BufferUsage::ELEMENT>(cube->ebo);
    ctx->bufferData<BufferUsage::ELEMENT, BufferTarget::STATIC_DRAW>((GLsizeiptr)(sizeof(unsigned int) * indices.size()), indices.data());
    ctx->bufferUse<BufferUsage::ELEMENT>(0);
    ctx->vaoUse(0);

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
            ctx->vaoUse(finalMesh->vao);

            // VBOs
            for (auto& attribute : primitive.attributes) {
                auto accessor = model.accessors[attribute.second];
                auto bufferView = model.bufferViews[accessor.bufferView];
                auto buffer = model.buffers[bufferView.buffer];
                int stride = accessor.ByteStride(bufferView);

                if (bufferView.target != (int)BufferUsage::ARRAY) {
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
                        ctx->bufferUse<BufferUsage::ARRAY>(it->second);
                        ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>((GLsizeiptr)(bufferView.byteLength), buffer.data.data() + bufferView.byteOffset);
                    }

                    ctx->bufferAttribute(index, (GLenum)accessor.componentType, (GLint)accessor.type, (GLsizei)stride, (GLvoid*)accessor.byteOffset);
                }
            }

            // EBO (indices)
            ctx->bufferUse<BufferUsage::ELEMENT>(finalMesh->ebo);
            auto accessor = model.accessors[primitive.indices];
            auto indicesBufferView = model.bufferViews[accessor.bufferView];
            auto indicesBuffer = model.buffers[indicesBufferView.buffer];
            ctx->bufferData<BufferUsage::ELEMENT, BufferTarget::STATIC_DRAW>((GLsizeiptr)(indicesBufferView.byteLength), indicesBuffer.data.data() + indicesBufferView.byteOffset);
            ctx->bufferUse<BufferUsage::ELEMENT>(0);
            ctx->vaoUse(0);

            finalMesh->drawMode = primitive.mode;
            finalMesh->indicesCount = accessor.count;
            finalMesh->drawType = accessor.componentType;
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
    ctx->shaderUniform("isAxis", 0);
    ctx->vaoUse(vao);
    ctx->bufferUse<BufferUsage::ELEMENT>(ebo);
    if (isDebugMode) {
        ctx->draw<DrawMethod::ELEMENT>(DrawParameters(GL_LINES, (GLenum)drawType, (GLsizei)indicesCount, nullptr));
    }
    else {
        ctx->draw<DrawMethod::ELEMENT>(DrawParameters((GLenum)drawMode, (GLenum)drawType, (GLsizei)indicesCount, nullptr));
    }
    ctx->bufferUse<BufferUsage::ELEMENT>(0);

    if (isDebugMode) {
        ctx->vaoUse(axisVao);
        ctx->shaderUniform("isAxis", 1);
        ctx->draw<DrawMethod::ARRAY>(DrawParameters(GL_LINES, 0, 6));
    }

    ctx->vaoUse(0);

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