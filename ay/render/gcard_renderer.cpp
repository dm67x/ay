#include "gcard_renderer.h"
#include "gcard.h"
#include "embedding.h"
#include "log.h"

GCardRenderer::GCardRenderer(const GCard& card, const Embedding& embedding)
    : m_gcard{ card },
    m_embedding{ embedding },
    m_vertices{},
    m_indices{},
    m_vbo{ 0 },
    m_vao{ 0 },
    m_ebo{ 0 }
{
    glGenVertexArrays(1, &m_vao);
    glCheckError();
    glGenBuffers(1, &m_vbo);
    glCheckError();
    glGenBuffers(1, &m_ebo);
    glCheckError();
}

GCardRenderer::~GCardRenderer()
{
    glDeleteBuffers(1, &m_vbo);
    glCheckError();
    glDeleteVertexArrays(1, &m_vao);
    glCheckError();
}

void GCardRenderer::build()
{
    // Get vertices
    for (auto vertex : m_embedding.vertices()) {
        m_vertices.push_back(*vertex.second);
    }

    // Get all faces
    for (auto face : m_gcard.faces()) {
        // Get each time 3 vertices (triangle)
        size_t i = 0;
        while (i < face.size() - 2) {
            Strand b1 = face[0];
            Strand b2 = face[i + 1];
            Strand b3 = face[i + 2];

            Vertex v1 = m_embedding[b1];
            Vertex v2 = m_embedding[b2];
            Vertex v3 = m_embedding[b3];

            // Find vertices indexes
            auto it = std::find(m_vertices.begin(), m_vertices.end(), v1);
            m_indices.push_back(static_cast<GLuint>(it - m_vertices.begin()));

            it = std::find(m_vertices.begin(), m_vertices.end(), v2);
            m_indices.push_back(static_cast<GLuint>(it - m_vertices.begin()));

            it = std::find(m_vertices.begin(), m_vertices.end(), v3);
            m_indices.push_back(static_cast<GLuint>(it - m_vertices.begin()));

            i++;
        }
    }

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

void GCardRenderer::draw() const
{
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