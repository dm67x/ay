#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>

#include "Strand.hpp"
#include "Vertex.hpp"
#include "Ay.hpp"

class GCard;

class Embedding final
{
    const GCard& m_card;
    std::map<std::vector<Strand>, Vertex*> m_vertices;

public:
    AY_API Embedding(const GCard&);
    AY_API ~Embedding();

    AY_API Vertex& operator[](Strand);
    AY_API const Vertex& operator[](Strand) const;
    AY_API void reload();

    inline std::map<std::vector<Strand>, Vertex*> vertices() const {
        return m_vertices; 
    }
};