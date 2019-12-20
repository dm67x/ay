#pragma once

#include "Vertex.hpp"
#include "Transform.hpp"

#include <vector>
#include <string>
#include <glad/glad.h>
#include <cstdint>

class Mesh
{
    std::vector<std::vector<size_t>> m_faces;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;
    std::string m_materialName;

private:
    friend class Model;

public:
    Mesh();
    ~Mesh();

public:
    size_t addVertex(const Vertex&);
    void addFace(const std::vector<size_t>&);
    void build();
    void draw() const;

public:
    inline const std::vector<std::vector<size_t>>& faces() const
    { 
        return m_faces; 
    }

    inline const std::vector<Vertex>& vertices() const
    {
        return m_vertices;
    }
};