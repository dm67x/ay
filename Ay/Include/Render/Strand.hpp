#pragma once

struct Strand final
{
    Strand* phi1;
    Strand* phi2;
    Strand* phi_1;

    friend bool operator==(const Strand& s1, const Strand& s2) 
    {
        return 
            s1.phi1 == s2.phi1 && 
            s1.phi2 == s2.phi2 && 
            s1.phi_1 == s2.phi_1;
    }
};