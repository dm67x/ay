#pragma once

#include "entity.hpp"
#include "model/vertex.hpp"

#include <vector>
#include <string>
#include <glad/glad.h>
#include <cstdint>

class Material;

class Mesh : public Entity
{
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;
    GLenum m_mode;
    std::shared_ptr<Material> m_material;

private:
    friend class Scene;

public:
    Mesh();
    ~Mesh();

public:
    void build();
    void draw(const Shader&) const override;
};