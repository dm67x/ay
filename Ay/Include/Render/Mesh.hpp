#pragma once

#include "Ay.hpp"
#include "Vertex.hpp"
#include "Transform.hpp"

#include <vector>
#include <string>
#include <glad/glad.h>

class Mesh
{
    std::vector<std::vector<size_t>> m_faces;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;
    std::string m_materialName;

    friend class Model;

public:
    AY_API Mesh();
    AY_API ~Mesh();
    AY_API size_t addVertex(const Vertex&);
    AY_API void addFace(const std::vector<size_t>&);
    AY_API void build();
    AY_API void draw() const;

    inline const std::vector<std::vector<size_t>>& faces() const
    { 
        return m_faces; 
    }

    inline const std::vector<Vertex>& vertices() const
    {
        return m_vertices;
    }
};