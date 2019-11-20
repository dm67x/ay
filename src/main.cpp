#include "device.h"
#include "gcard.h"

#include <stdexcept>
#include <iostream>

int main()
{
    try {
        Device device{ 800, 800 };

        GCard tore;
        Strand base;

        base = tore.newFace(4);

        // Alpha2
        tore.alpha2(base, tore.alpha1(tore.alpha0(tore.alpha1(base))));
        tore.alpha2(tore.alpha1(base), tore.alpha1(tore.alpha0(base)));
        tore.alpha2(tore.alpha0(tore.alpha1(base)), tore.alpha0(tore.alpha1(tore.alpha0(base))));
        tore.alpha2(tore.alpha0(base), tore.alpha0(tore.alpha1(tore.alpha0(tore.alpha1(base)))));

        std::cout << tore.euler() << std::endl;
        std::cout << tore.vertices().size() << std::endl;

        for (auto vert : tore.vertices()) {
            for (auto v : vert) {
                std::cout << v << ", ";
            }
            std::cout << std::endl;
        }

        device.Run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    
    return 0;
}