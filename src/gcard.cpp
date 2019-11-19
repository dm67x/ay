#include "gcard.h"

GCard::GCard()
    : m_strands{},
    m_alpha0{},
    m_alpha1{},
    m_alpha2{}
{
}

Strand GCard::newStrand()
{
    Strand ns = m_strands.size();
    m_strands.push_back(ns);
    m_alpha0.push_back(ns);
    m_alpha1.push_back(ns);
    m_alpha2.push_back(ns);
    return ns;
}

Strand GCard::newFace(size_t side)
{
    Strand* strands = new Strand[side * 2];

    for (size_t i = 0; i < side * 2; i++) {
        strands[i] = newStrand();
    }

    // Alpha0
    for (size_t i = 0; i < side * 2; i += 2) {
        alpha0(strands[i], strands[i + 1]);
        alpha0(strands[i + 1], strands[i]);
    }

    return strands[0];
}

Strand GCard::alpha0(Strand src) const
{
    return m_alpha0[src];
}

Strand GCard::alpha1(Strand src) const
{
    return m_alpha1[src];
}

Strand GCard::alpha2(Strand src) const
{
    return m_alpha2[src];
}

void GCard::alpha0(Strand src, Strand dest)
{
    m_alpha0[src] = dest;
}

void GCard::alpha1(Strand src, Strand dest)
{
    m_alpha1[src] = dest;
}

void GCard::alpha2(Strand src, Strand dest)
{
    m_alpha2[src] = dest;
}

std::vector<std::vector<Strand>> GCard::a2oa1() const
{
    std::vector<std::vector<Strand>> result = {};
    std::vector<Strand> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand src = m_strands[i];
        Strand current = alpha1(alpha2(src));

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = alpha1(alpha2(current));
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

std::vector<std::vector<Strand>> GCard::a0oa2() const
{
    std::vector<std::vector<Strand>> result = {};
    std::vector<Strand> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand src = m_strands[i];
        Strand current = alpha0(alpha2(src));

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = alpha0(alpha2(current));
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

std::vector<std::vector<Strand>> GCard::a1oa0() const
{
    std::vector<std::vector<Strand>> result = {};
    std::vector<Strand> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand src = m_strands[i];
        Strand current = alpha1(alpha0(src));

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = alpha1(alpha0(current));
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

std::vector<std::vector<Strand>> GCard::vertices() const
{
    std::vector<std::vector<Strand>> result = {};
    std::vector<Strand> markeds = {};

    size_t i = 0;
    while (markeds.size() != m_strands.size()) {
        Strand src = m_strands[i];
        Strand current = alpha1(src);

        auto findResult = std::find(markeds.begin(), markeds.end(), src);
        if (findResult == markeds.end()) {
            std::vector<Strand> vstrand = {};
            vstrand.push_back(src);
            markeds.push_back(src);

            while (current != src) {
                auto ns = alpha1(current);
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
    return (int)((a2oa1().size() + 
        a0oa2().size() + 
        a1oa0().size() - 
        m_strands.size()) / 2);
}