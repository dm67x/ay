#include "mesh_factory.hpp"
#include <spdlog/spdlog.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <algorithm>

Mesh* MeshFactory::plane(Context* ctx) {
    Mesh* plane = new Mesh(ctx);
    plane->vertices = {
        Mesh::Vertex(Vec3(-1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 0.f),
        Mesh::Vertex(Vec3(1.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 0.f),
        Mesh::Vertex(Vec3(1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.f, 1.f),
        Mesh::Vertex(Vec3(-1.f, -1.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.f, 1.f)
    };
    plane->indices = { 0, 2, 1, 0, 3, 2 };
    plane->build();
    return plane;
}

Mesh* MeshFactory::cube(Context* ctx) {
    Mesh* cube = new Mesh(ctx);
    cube->vertices = {
        Mesh::Vertex(Vec3(-1.f, 1.f, -1.f), Vec3(), 0.25f, 0.f), // v1
        Mesh::Vertex(Vec3(-1.f, 1.f, 1.f), Vec3(), 0.75f, 0.f), // v2
        Mesh::Vertex(Vec3(1.f, 1.f, 1.f), Vec3(), 0.75f, 0.25f), // v3
        Mesh::Vertex(Vec3(1.f, 1.f, -1.f), Vec3(), 0.25f, 0.25f), // v4
        Mesh::Vertex(Vec3(-1.f, -1.f, 1.f), Vec3(), 0.75f, 0.75f), // v5
        Mesh::Vertex(Vec3(-1.f, -1.f, -1.f), Vec3(), 0.25f, 0.75f), // v6
        Mesh::Vertex(Vec3(1.f, -1.f, -1.f), Vec3(), 0.25f, 0.5f), // v7
        Mesh::Vertex(Vec3(1.f, -1.f, 1.f), Vec3(), 0.75f, 0.5f), // v8
        
        Mesh::Vertex(Vec3(-1.f, 1.f, -1.f), Vec3(), 0.f, 0.25f), // v12
        Mesh::Vertex(Vec3(-1.f, 1.f, -1.f), Vec3(), 0.25f, 1.f), // v13
        Mesh::Vertex(Vec3(-1.f, 1.f, 1.f), Vec3(), 1.f, 0.25f), // v22
        Mesh::Vertex(Vec3(-1.f, 1.f, 1.f), Vec3(), 0.75f, 1.f), // v23
        Mesh::Vertex(Vec3(-1.f, -1.f, 1.f), Vec3(), 1.f, 0.5f), // v52
        Mesh::Vertex(Vec3(-1.f, -1.f, -1.f), Vec3(), 0.f, 0.5f) // v62
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
    cube->build();
    return cube;
}

bool MeshFactory::computeMesh(
    const tinyobj::shape_t& shape, 
    const tinyobj::attrib_t& attrib, 
    std::vector<Mesh::Vertex>* vertices, 
    std::vector<unsigned int>* indices) 
{
    bool normalExists = false;
    size_t index_offset = 0;

    for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
        size_t fv = shape.mesh.num_face_vertices[f];
        for (size_t v = 0; v < fv; v++) {
            tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
            tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
            tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
            tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
            tinyobj::real_t nx = 0.f;
            tinyobj::real_t ny = 0.f;
            tinyobj::real_t nz = 0.f;
            tinyobj::real_t tx = 0.f;
            tinyobj::real_t ty = 0.f;

            if (idx.normal_index > 0) {
                nx = attrib.normals[3 * idx.normal_index + 0];
                ny = attrib.normals[3 * idx.normal_index + 1];
                nz = attrib.normals[3 * idx.normal_index + 2];
                normalExists = true;
            }

            if (idx.texcoord_index > 0) {
                tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                ty = attrib.texcoords[2 * idx.texcoord_index + 1];
            }

            Mesh::Vertex vert(Vec3(vx, vy, vz), Vec3(nx, ny, nz), tx, ty);
            auto it = std::find(vertices->begin(), vertices->end(), vert);
            if (it != vertices->end()) {
                const size_t index = it - vertices->begin();
                indices->push_back(static_cast<unsigned int>(index));
            }
            else {
                vertices->push_back(vert);
                indices->push_back(static_cast<unsigned int>(vertices->size() - 1));
            }
        }
        index_offset += fv;
    }

    return normalExists;
}

std::future<MeshFactory::AsyncMesh*> MeshFactory::fromFile(const std::string& filename) {
    return std::async(std::launch::async, [=]() {
        AsyncMesh* parent = new AsyncMesh();
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string warn, err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

        if (!warn.empty()) {
            spdlog::warn(warn);
        }

        if (!err.empty()) {
            spdlog::error(err);
        }

        if (!ret) {
            return parent;
        }

        std::vector<std::vector<Mesh::Vertex>*> verticesList;
        std::vector<std::vector<unsigned int>*> indicesList;
        std::vector<std::future<bool>> futures;

        for (size_t s = 0; s < shapes.size(); s++) {
            AsyncMesh* mesh = new AsyncMesh();

            verticesList.push_back(new std::vector<Mesh::Vertex>());
            indicesList.push_back(new std::vector<unsigned int>());
            futures.push_back(std::async(computeMesh, shapes[s], attrib, verticesList[s], indicesList[s]));

            parent->children.push_back(mesh);
        }

        for (auto it = futures.begin(); it != futures.end(); it++) {
            bool res = it->get();
            const size_t index = it - futures.begin();
            auto mesh = static_cast<AsyncMesh*>(parent->children[index]);
            mesh->vertices = *verticesList[index];
            mesh->indices = *indicesList[index];
            mesh->computeNormals = res;
            delete verticesList[index];
            delete indicesList[index];
        }

        spdlog::info("Mesh {} loaded successfully", filename);
        return parent;
    });
}