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

void GCard::phi1Sew(Strand src, Strand dest)
{
    m_phi1[src] = dest;
}

void GCard::phi1Unsew(Strand src)
{
    m_phi1[src] = src;
}

void GCard::phi2Sew(Strand src, Strand dest)
{
    m_phi2[src] = dest;
}

void GCard::phi2Unsew(Strand src)
{
    m_phi2[src] = src;
}