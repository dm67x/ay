#pragma once

#include "object.hpp"
#include "math.hpp"

class VertexArrayObject;
class Buffer;

struct Vertex {
    Vec3 position;
    Vec3 normal;
    float u;
    float v;
};

class Mesh : public Object {
    VertexArrayObject* vao;
    Buffer* ebo;
    Buffer* vbo;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool isBuilded;

    friend class MeshFactory;

public:
    ///
    /// @brief Constructor
    /// @param ctx Context pointer
    ///
    Mesh(Context* ctx);

    ///
    /// @brief Destructor
    ///
    ~Mesh() override;

    ///
    /// @brief Update called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    void update(float deltaTime) override;

    ///
    /// @brief Render called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    void render(float deltaTime) override;

private:
    ///
    /// @brief Build the mesh
    ///
    void build();
};