#include "Render/Embedding.hpp"
#include "Render/GCard.hpp"

#include <stdexcept>

Embedding::Embedding(const GCard& card)
    : m_card{ card },
    m_vertices{}
{
    reload();
}

Embedding::~Embedding()
{
    for (auto vertex : m_vertices) {
        delete vertex.second;
    }
}

Vertex& Embedding::operator[](Strand* strand)
{
    for (auto vertex : m_vertices) {
        if (std::find(vertex.first.begin(), 
            vertex.first.end(), strand) != vertex.first.end())
        {
            return *vertex.second;
        }
    }

    throw std::runtime_error("Strand didn't exist inside vertices");
}

const Vertex& Embedding::operator[](Strand* strand) const
{
    for (auto vertex : m_vertices) {
        if (std::find(vertex.first.begin(),
            vertex.first.end(), strand) != vertex.first.end())
        {
            return *vertex.second;
        }
    }

    throw std::runtime_error("Strand didn't exist inside vertices");
}

void Embedding::reload()
{
    m_vertices.clear();
    for (auto vertex : m_card.vertices()) {
        m_vertices.insert(std::make_pair(vertex, new Vertex));
    }
}