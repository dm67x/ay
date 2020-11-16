#pragma once

#include "object.hpp"
#include "math.hpp"
#include "context.hpp"
#include "material.hpp"
#include "tiny_gltf.h"
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

    VAO vao;
    Buffer ebo;
    std::map<int, Buffer> vbos;
    Material material;
    int drawMode;
    int drawType;
    size_t indicesCount;
    VAO axisVao;
    Buffer axisBuffer; // x, y, z
    bool isDebugMode; // for debugging

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

    ///
    /// @brief Activate or deactivate debug mode
    /// @param value Bool
    ///
    inline void setDebug(bool value) {
        isDebugMode = value;

        for (auto child : children) {
            auto childMesh = static_cast<Mesh*>(child);
            childMesh->setDebug(value);
        }
    }

private:
    ///
    /// @brief Compute the normals
    /// @param vertices (In) vertices
    /// @param indices (In) indices
    /// @param output (Out) updated vertices
    /// 
    static void computeNormals(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Vertex>& output);

    void processNode(tinygltf::Model model, tinygltf::Node node);

    void processMesh(tinygltf::Model model, tinygltf::Mesh mesh);

    void processMaterial(tinygltf::Material mat);
};