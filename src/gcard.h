#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "strand.h"

class GCard final
{
    std::vector<Strand> m_strands;
    std::vector<Strand> m_phi1;
    std::vector<Strand> m_phi2;

public:
    GCard();
    ~GCard() = default;

    Strand newStrand();
    void phi1Sew(Strand, Strand);
    void phi1Unsew(Strand);
    void phi2Sew(Strand, Strand);
    void phi2Unsew(Strand);
};