#include "mesh.hpp"
#include "context.hpp"
#include <iostream>

Mesh::Mesh(Context* ctx) 
    : Object(ctx), 
    vao(nullptr), 
    ebo(nullptr), 
    vbo(nullptr), 
    vertices(), 
    indices(),
    isBuilded(false)
{
    vao = ctx->vertexArrayObjectNew();
    ebo = ctx->bufferNew();
    vbo = ctx->bufferNew();
}

Mesh::~Mesh() {
    delete vao;
    delete vbo;
    delete ebo;
}

void Mesh::update(float deltaTime) {
    (void)deltaTime;
}

void Mesh::render(float deltaTime) {
    (void)deltaTime;

    Mat4 _transform = transform.getTransform();
    if (parent != nullptr) {
        _transform = _transform * parent->transform.getTransform();
    }

    ctx->shaderUniform("modelMatrix", _transform);
    vao->use();
    ebo->use(BufferMode::ELEMENT_ARRAY);
    vao->drawElements(DrawMode::TRIANGLES, indices.size(), DrawType::UNSIGNED_INT, nullptr);
    Buffer::reset(BufferMode::ELEMENT_ARRAY);
    VertexArrayObject::reset();
}

void Mesh::build() {
    if (isBuilded) return;

    vao->use();
    vbo->use(BufferMode::ARRAY);
    vbo->set(BufferMode::ARRAY, sizeof(Vertex) * vertices.size(), vertices.data(), BufferTarget::STATIC_DRAW);
    vbo->attribArray(0, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
    vbo->attribArray(1, 3, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    vbo->attribArray(2, 2, AttribType::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, u));
    ebo->use(BufferMode::ELEMENT_ARRAY);
    ebo->set(BufferMode::ELEMENT_ARRAY, sizeof(unsigned int) * indices.size(), indices.data(), BufferTarget::STATIC_DRAW);
    Buffer::reset(BufferMode::ELEMENT_ARRAY);
    VertexArrayObject::reset();

    isBuilded = true;
}

void Mesh::computeNormals() {
    // Get faces
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int v1i = indices[i];
        unsigned int v2i = indices[i+1];
        unsigned int v3i = indices[i+2];

        Vec3 v1 = vertices[v1i].position;
        Vec3 v2 = vertices[v2i].position;
        Vec3 v3 = vertices[v3i].position;

        Vec3 v1v2 = v2 - v1;
        Vec3 v1v3 = v3 - v1;

        Vec3 normal = v1v2 * v1v3;
        normal = normal.normalize();

        vertices[v1i].normal = normal;
        vertices[v2i].normal = normal;
        vertices[v3i].normal = normal;
    }
}