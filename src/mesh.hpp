#pragma once

#include "object.hpp"
#include "math.hpp"
#include "context.hpp"
#include <map>
#include <string>

class Mesh : public Object {
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
    std::map<int, Buffer*> vbos;
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
    static Mesh* fromFile(Context* ctx, const std::string& filename);

    ///
    /// @brief Render called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    void render(float deltaTime) override;

private:
    ///
    /// @brief Compute the normals
    /// @param vertices (In) vertices
    /// @param indices (In) indices
    /// @param output (Out) updated vertices
    /// 
    static void computeNormals(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Vertex>& output);
};