#include "Render/GCard.hpp"
#include "Render/Strand.hpp"

#include <fstream>

GCard::GCard()
    : m_strands{}
{
}

GCard::~GCard()
{
    for (auto strand : m_strands) {
        delete strand;
    }
}

GCard& GCard::operator=(const GCard& card)
{
    if (this == &card) 
        return *this;
    
    m_strands = card.m_strands;
    return *this;
}

Strand* GCard::operator[](int index)
{
    assert(index < m_strands.size());
    return m_strands[index];
}

Strand* GCard::newStrand()
{
    Strand* ns = new Strand;
    m_strands.push_back(ns);
    return ns;
}

Strand* GCard::newFace(size_t side)
{
    Strand** strands = new Strand*[side];

    // Create strands
    for (size_t i = 0; i < side; i++) {
        strands[i] = newStrand();
    }

    // Phi1
    for (size_t i = 1; i < side; i++) {
        strands[i]->phi1 = strands[(i + 1) % side];
        strands[i]->phi_1 = strands[i - 1];
    }

    if (side >= 2) {
        strands[0]->phi1 = strands[1];
        strands[0]->phi_1 = strands[side - 1];
    }

    return strands[0];
}

std::vector<std::vector<Strand*>> GCard::vertices() const
{
    std::vector<std::vector<Strand*>> result = {};
    std::vector<Strand*> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand* src = m_strands[i];
        Strand* current = src->phi2->phi1;

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand*> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = current->phi2->phi1;
                vstrand.push_back(current);
                markeds.push_back(current);
                current = ns;
            }

            result.push_back(vstrand);
        }
        else {
            src = m_strands[++i];
        }
    }

    return result;
}

std::vector<std::vector<Strand*>> GCard::edges() const
{
    std::vector<std::vector<Strand*>> result = {};
    std::vector<Strand*> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand* src = m_strands[i];
        Strand* current = src->phi2;

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand*> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = current->phi2;
                vstrand.push_back(current);
                markeds.push_back(current);
                current = ns;
            }

            result.push_back(vstrand);
        }
        else {
            src = m_strands[++i];
        }
    }

    return result;
}

std::vector<std::vector<Strand*>> GCard::faces() const
{
    std::vector<std::vector<Strand*>> result = {};
    std::vector<Strand*> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand* src = m_strands[i];
        Strand* current = src->phi1;

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand*> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = current->phi1;
                vstrand.push_back(current);
                markeds.push_back(current);
                current = ns;
            }

            result.push_back(vstrand);
        }
        else {
            src = m_strands[++i];
        }
    }

    return result;
}

int GCard::euler() const
{
    return static_cast<int>(vertices().size() 
        - edges().size() 
        + faces().size());
}

void GCard::graph(const std::string& filename) const
{
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "digraph gcard_graph {\n";

        auto faces = this->faces();
        auto vertices = this->vertices();
        auto edges = this->edges();

        std::vector<int> face_ids = {};

        // Edge
        int edge_id = 0;
        for (auto edge : edges) {
            for (auto e : edge) {
                // Face
                int face_id = 0;
                for (auto f : faces) {
                    if (std::find(f.begin(), f.end(), e) != f.end()) {
                        file << "\tface_" << face_id << 
                            " -> edge_" << edge_id << ";\n";
                    }

                    if (std::find(face_ids.begin(), 
                        face_ids.end(), face_id) == face_ids.end()) 
                    {
                        file << "\tS -> face_" << face_id << ";\n";
                        face_ids.push_back(face_id);
                    }

                    face_id++;
                }

                // Vertex
                int vertex_id = 0;
                for (auto v : vertices) {
                    if (std::find(v.begin(), v.end(), e) != v.end()) {
                        file << "\tedge_" << edge_id << 
                            " -> vertex_" << vertex_id << ";\n";
                    }
                    vertex_id++;
                }
            }
            edge_id++;
        }

        file << "}\n";

        file.close();
    }
}