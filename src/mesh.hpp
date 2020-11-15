#pragma once

#include "object.hpp"
#include "math.hpp"
#include "context.hpp"
#include <map>

struct Vertex {
    Vec3 position;
    Vec3 normal;
    float u;
    float v;

    ///
    /// @brief Constructor
    /// @param position Vec3
    /// @param normal Vec3
    /// @param u Texture coordinates (u, v)
    /// @param v Texture coordinates (u, v)
    ///
    Vertex(Vec3 position, Vec3 normal, float u, float v)
        : position(position), normal(normal), u(u), v(v)
    {
    }

    ///
    /// @brief Equal operator
    /// @param v Vertex
    /// @return True if equal false otherwise
    ///
    inline bool operator==(const Vertex& v1) {
        return position == v1.position && normal == v1.normal && u == v1.u && v == v1.v;
    }
};

class Mesh : public Object {
    friend struct MeshFactory;

    VertexArrayObject* vao;
    Buffer* ebo;
    std::map<int, Buffer*> vbos;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    DrawMode drawMode;
    DrawType drawType;
    size_t indicesCount;

private:
    ///
    /// @brief Constructor
    /// @param ctx Context pointer
    ///
    Mesh(Context* ctx);

public:
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
    /// @brief Compute the normals
    /// 
    void computeNormals();
};