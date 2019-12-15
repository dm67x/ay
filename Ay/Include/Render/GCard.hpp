#pragma once

#include "Ay.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Strand;

class GCard final
{
    std::vector<Strand*> m_strands;

public:
    AY_API GCard();
    AY_API ~GCard();

    AY_API GCard& operator=(const GCard&);
    AY_API Strand* operator[](int);

    AY_API Strand* newStrand();
    AY_API Strand* newFace(size_t);

    AY_API std::vector<std::vector<Strand*>> vertices() const;
    AY_API std::vector<std::vector<Strand*>> edges() const;
    AY_API std::vector<std::vector<Strand*>> faces() const;

    AY_API int euler() const;
    AY_API void graph(const std::string&) const;
};