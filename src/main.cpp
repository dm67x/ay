#include "device.h"
#include "gcard.h"

#include <stdexcept>
#include <iostream>

int main()
{
    try {
        Device device{ 800, 800 };

        GCard tore;
        Strand b1, b2, b3, b4, b5, b6, b7, b8;

        // Brins
        b1 = tore.newStrand();
        b2 = tore.newStrand();
        b3 = tore.newStrand();
        b4 = tore.newStrand();
        b5 = tore.newStrand();
        b6 = tore.newStrand();
        b7 = tore.newStrand();
        b8 = tore.newStrand();

        // Alpha0
        tore.alpha0(b1, b2);
        tore.alpha0(b3, b4);
        tore.alpha0(b5, b6);
        tore.alpha0(b7, b8);
        tore.alpha0(b2, b1);
        tore.alpha0(b4, b3);
        tore.alpha0(b6, b5);
        tore.alpha0(b8, b7);

        // Alpha1
        tore.alpha1(b1, b8);
        tore.alpha1(b2, b3);
        tore.alpha1(b3, b2);
        tore.alpha1(b4, b5);
        tore.alpha1(b5, b4);
        tore.alpha1(b6, b7);
        tore.alpha1(b7, b6);
        tore.alpha1(b8, b1);

        // Alpha2
        tore.alpha2(b1, b6);
        tore.alpha2(b2, b5);
        tore.alpha2(b3, b8);
        tore.alpha2(b4, b7);
        tore.alpha2(b5, b2);
        tore.alpha2(b6, b1);
        tore.alpha2(b7, b4);
        tore.alpha2(b8, b3);

        std::cout << tore.euler() << std::endl;

        device.Run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    
    return 0;
}