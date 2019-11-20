#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "strand.h"

class GCard
{
    std::vector<Strand> m_strands;
    std::vector<Strand> m_alpha0;
    std::vector<Strand> m_alpha1;
    std::vector<Strand> m_alpha2;

public:
    GCard();
    ~GCard() = default;

    Strand newStrand();
    Strand newFace(size_t);
    Strand alpha0(Strand) const;
    Strand alpha1(Strand) const;
    Strand alpha2(Strand) const;

    void alpha0(Strand, Strand);
    void alpha1(Strand, Strand);
    void alpha2(Strand, Strand);

    std::vector<std::vector<Strand>> a2oa1() const;
    std::vector<std::vector<Strand>> a0oa2() const;
    std::vector<std::vector<Strand>> a1oa0() const;
    std::vector<std::vector<Strand>> vertices() const;
    std::vector<std::vector<Strand>> edges() const;

    int euler() const;
};