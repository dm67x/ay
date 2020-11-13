#include "mesh.hpp"
#include "context.hpp"
#include <iostream>

Mesh::Mesh(Context* ctx) 
    : Object(ctx), 
    vao(nullptr), 
    ebo(nullptr), 
    vbo(nullptr), 
    vertices(), 
    indices() 
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
    vao->drawElements(DrawMode::TRIANGLES, indices.size(), DrawType::UNSIGNED_INT, 0);
    Buffer::reset(BufferMode::ELEMENT_ARRAY);
    VertexArrayObject::reset();
}

void Mesh::build() const {
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
}