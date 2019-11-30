#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>

#include "strand.h"
#include "vertex.h"

class GCard;

class Embedding final
{
    const GCard& m_card;
    std::map<std::vector<Strand>, Vertex*> m_vertices;

public:
    Embedding(const GCard&);
    ~Embedding();

    Vertex& operator[](Strand);
    void reload();

    inline std::map<std::vector<Strand>, Vertex*> vertices() const { 
        return m_vertices; 
    }
};