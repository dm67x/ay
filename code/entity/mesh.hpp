#pragma once

#include "entity.hpp"
#include "model/vertex.hpp"

#include <vector>
#include <string>
#include <glad/glad.h>
#include <cstdint>

class Mesh : public Entity
{
    //std::vector<std::vector<size_t>> m_faces;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;
    // Material

private:
    friend class Scene;

public:
    Mesh();
    ~Mesh();

public:
    /*size_t addVertex(const Vertex&);
    void addFace(const std::vector<size_t>&);*/
    void build();
    void draw(const Shader&) const override;

public:
    /*inline const std::vector<std::vector<size_t>>& faces() const
    { 
        return m_faces; 
    }

    inline const std::vector<Vertex>& vertices() const
    {
        return m_vertices;
    }*/
};