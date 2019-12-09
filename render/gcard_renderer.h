#pragma once

#include <glad/glad.h>
#include <vector>

#include "vertex.h"
#include "scene/entity.h"

class GCard;
class Embedding;

class GCardRenderer final : public SceneEntity
{
    const GCard& m_gcard;
    const Embedding& m_embedding;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;

public:
    GCardRenderer(const GCard&, const Embedding&);
    ~GCardRenderer();

    void draw() const override;
};