#pragma once

#include "object.hpp"
#include "math.hpp"

class VertexArrayObject;
class Buffer;

class Mesh : public Object {
    friend struct MeshFactory;

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

    VertexArrayObject* vao;
    Buffer* ebo;
    Buffer* vbo;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool isBuilded;

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
    /// @brief Build the mesh
    ///
    void build();

    ///
    /// @brief Compute the normals
    /// 
    void computeNormals();
};