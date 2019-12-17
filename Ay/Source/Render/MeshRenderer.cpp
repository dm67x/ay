#include "Render/MeshRenderer.hpp"
#include "Render/Mesh.hpp"
#include "Render/MaterialManager.hpp"
#include "Log.hpp"

MeshRenderer::MeshRenderer(const Mesh& mesh, const std::string& material)
    : m_mesh{ mesh },
    m_indices{},
    m_vbo{ 0 },
    m_vao{ 0 },
    m_ebo{ 0 },
    m_material{ MaterialManager::instance().get(material) }
{
    glGenVertexArrays(1, &m_vao);
    glCheckError();
    glGenBuffers(1, &m_vbo);
    glCheckError();
    glGenBuffers(1, &m_ebo);
    glCheckError();
}

MeshRenderer::~MeshRenderer()
{
    glDeleteBuffers(1, &m_vbo);
    glCheckError();
    glDeleteVertexArrays(1, &m_vao);
    glCheckError();
}

void MeshRenderer::build()
{
    auto vertices = m_mesh.vertices();
    auto faces = m_mesh.faces();

    for (auto face : faces) {
        for (auto f : face) {
            m_indices.push_back(static_cast<GLuint>(f));
        }
    }

    glBindVertexArray(m_vao);
    glCheckError();

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glCheckError();

    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(), GL_STATIC_DRAW);
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

void MeshRenderer::draw(const ShaderProgram& program) const
{
    program.uniform("modelMatrix", transform());

    if (m_material)
        m_material->use(program);

    glBindVertexArray(m_vao);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glCheckError();
    glDrawElements(GL_TRIANGLES,
        (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();
    glBindVertexArray(0);
    glCheckError();
}