#pragma once

#include "Ay.hpp"
#include "Vertex.hpp"

#include <vector>
#include <string>

class Mesh
{
    std::vector<std::vector<size_t>> m_faces;
    std::vector<Vertex> m_vertices;

public:
    AY_API Mesh();

    AY_API size_t addVertex(const Vertex&);
    AY_API void addFace(const std::vector<size_t>&);
    AY_API void load(const std::string&);

    inline const std::vector<std::vector<size_t>>& faces() const
    { 
        return m_faces; 
    }

    inline const std::vector<Vertex>& vertices() const
    {
        return m_vertices;
    }
};