#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "strand.h"

class GCard
{
    std::vector<Strand> m_strands;
    std::vector<Strand> m_phi1; // faces
    std::vector<Strand> m_phi2; // edges

public:
    GCard();
    ~GCard() = default;

    Strand newStrand();
    Strand newFace(size_t);

    Strand phi1(Strand) const;
    Strand phi2(Strand) const;

    void phi1(Strand, Strand);
    void phi2(Strand, Strand);

    std::vector<std::vector<Strand>> vertices() const;
    std::vector<std::vector<Strand>> edges() const;
    std::vector<std::vector<Strand>> faces() const;

    int euler() const;
};