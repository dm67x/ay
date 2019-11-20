#pragma once

#include <glad/glad.h>
#include <vector>

#include "vertex.h"

class GCard;

class GCardRenderer final
{
    const GCard& m_gcard;
    std::vector<Vertex> m_vertices;
    GLuint m_vbo;
    GLuint m_vao;

public:
    GCardRenderer(const GCard&, const std::vector<Vertex>&);
    ~GCardRenderer();

    void render() const;
};