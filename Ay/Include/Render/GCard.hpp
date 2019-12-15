#pragma once

#include "Strand.hpp"
#include "Ay.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <string>

class GCard
{
    std::vector<Strand> m_strands;
    std::vector<Strand> m_phi1; // faces
    std::vector<Strand> m_phi2; // edges

public:
    AY_API GCard();
    AY_API ~GCard() = default;

    AY_API GCard& operator=(const GCard&);

    AY_API Strand newStrand();
    AY_API Strand newFace(size_t);

    AY_API Strand phi1(Strand) const;
    AY_API Strand phi_1(Strand) const;
    AY_API Strand phi2(Strand) const;

    AY_API void phi1(Strand, Strand);
    AY_API void phi2(Strand, Strand);

    AY_API std::vector<std::vector<Strand>> vertices() const;
    AY_API std::vector<std::vector<Strand>> edges() const;
    AY_API std::vector<std::vector<Strand>> faces() const;

    AY_API int euler() const;
    AY_API void graph(const std::string&) const;
};