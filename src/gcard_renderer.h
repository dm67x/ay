#pragma once

#include <glad/glad.h>
#include <vector>

#include "vertex.h"

class GCard;
class Embedding;

class GCardRenderer final
{
    const GCard& m_gcard;
    Embedding& m_embedding;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;

public:
    GCardRenderer(const GCard&, Embedding&);
    ~GCardRenderer();

    void render() const;
};