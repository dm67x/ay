#include "mesh.hpp"
#include "log.hpp"
#include "rendering/shader.hpp"
#include "rendering/material.hpp"

Mesh::MeshPrimitive::MeshPrimitive()
    : m_vertices{},
    m_indices{},
    m_vbo{ 0 },
    m_vao{ 0 },
    m_ebo{ 0 },
    m_mode{ GL_TRIANGLES },
    m_material{ nullptr }
{
    glGenVertexArrays(1, &m_vao);
    glCheckError();
    glGenBuffers(1, &m_vbo);
    glCheckError();
    glGenBuffers(1, &m_ebo);
    glCheckError();
}

Mesh::MeshPrimitive::~MeshPrimitive()
{
    glDeleteBuffers(1, &m_vbo);
    glCheckError();
    glDeleteVertexArrays(1, &m_vao);
    glCheckError();
}

Vertex& Mesh::MeshPrimitive::operator[](int index)
{
    return m_vertices.at(index);
}

void Mesh::MeshPrimitive::add(const Vertex& vert)
{
    m_vertices.push_back(vert);
}

void Mesh::MeshPrimitive::add(GLuint indice)
{
    m_indices.push_back(indice);
}

void Mesh::MeshPrimitive::set(GLenum mode)
{
    m_mode = mode;
}

void Mesh::MeshPrimitive::set(const Material& mat)
{
    m_material = std::make_shared<Material>(mat);
}

void Mesh::MeshPrimitive::build() const
{
    glBindVertexArray(m_vao);
    glCheckError();

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glCheckError();

    glBufferData(GL_ARRAY_BUFFER,
        m_vertices.size() * sizeof(Vertex),
        m_vertices.data(), GL_STATIC_DRAW);
    glCheckError();

    // position
    glEnableVertexAttribArray(0);
    glCheckError();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glCheckError();

    // normal
    glEnableVertexAttribArray(1);
    glCheckError();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glCheckError();

    // uv
    glEnableVertexAttribArray(2);
    glCheckError();
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glCheckError();

    // color
    glEnableVertexAttribArray(3);
    glCheckError();
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, color));
    glCheckError();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glCheckError();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        m_indices.size() * sizeof(GLuint),
        m_indices.data(), GL_STATIC_DRAW);
    glCheckError();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();

    glBindVertexArray(0);
    glCheckError();
}

void Mesh::MeshPrimitive::draw(const Shader& shader) const
{
    if (m_material)
        m_material->use(shader);

    glBindVertexArray(m_vao);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glCheckError();
    glDrawElements(m_mode,
        (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();
    glBindVertexArray(0);
    glCheckError();
}

Mesh::Mesh(const std::string& name)
    : Entity(name),
    m_primitives{}
{
}

std::shared_ptr<Mesh::MeshPrimitive>& Mesh::create()
{
    m_primitives.push_back(std::make_shared<Mesh::MeshPrimitive>());
    return m_primitives.at(m_primitives.size() - 1);
}

void Mesh::build()
{
    for (auto primitive : m_primitives) {
        primitive->build();
    }
}

void Mesh::draw(const Shader& shader) const
{
    for (auto primitive : m_primitives) {
        shader.uniform("modelMatrix", transform() * primitive->transform());
        primitive->draw(shader);
    }
}