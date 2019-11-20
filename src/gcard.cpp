#include "gcard.h"

GCard::GCard()
    : m_strands{},
    m_phi1{},
    m_phi2{}
{
}

Strand GCard::newStrand()
{
    Strand ns = m_strands.size();
    m_strands.push_back(ns);
    m_phi1.push_back(ns);
    m_phi2.push_back(ns);
    return ns;
}

Strand GCard::newFace(size_t side)
{
    Strand* strands = new Strand[side];

    // Create strands
    for (size_t i = 0; i < side; i++) {
        strands[i] = newStrand();
    }

    // Phi1
    for (size_t i = 0; i < side; i++) {
        phi1(strands[i], strands[(i+1) % side]);
    }

    return strands[0];
}

Strand GCard::phi1(Strand src) const
{
    return m_phi1[src];
}

Strand GCard::phi2(Strand src) const
{
    return m_phi2[src];
}

void GCard::phi1(Strand src, Strand dest)
{
    m_phi1[src] = dest;
}

void GCard::phi2(Strand src, Strand dest)
{
    m_phi2[src] = dest;
    m_phi2[dest] = src;
}

std::vector<std::vector<Strand>> GCard::vertices() const
{
    std::vector<std::vector<Strand>> result = {};
    std::vector<Strand> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand src = m_strands[i];
        Strand current = phi1(phi2(src));

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = phi1(phi2(current));
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

std::vector<std::vector<Strand>> GCard::edges() const
{
    std::vector<std::vector<Strand>> result = {};
    std::vector<Strand> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand src = m_strands[i];
        Strand current = phi2(src);

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = phi2(current);
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

std::vector<std::vector<Strand>> GCard::faces() const
{
    std::vector<std::vector<Strand>> result = {};
    std::vector<Strand> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand src = m_strands[i];
        Strand current = phi1(src);

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = phi1(current);
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