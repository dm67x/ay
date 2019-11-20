#include "device.h"
#include "gcard.h"
#include "gcard_renderer.h"

#include <stdexcept>
#include <iostream>

GCard cube()
{
    GCard result;

    Strand d1, d2, d3, d4, d5, d6;
    d1 = result.newFace(4);
    d2 = result.newFace(4);
    d3 = result.newFace(4);
    d4 = result.newFace(4);
    d5 = result.newFace(4);
    d6 = result.newFace(4);

    glm::vec3 P[8] = {
        {-1, 1, -1},
        {1, 1, -1},
        {1, -1, -1},
        {-1, -1, -1},
        {-1, 1, 1},
        {1, 1, 1},
        {1, -1, 1},
        {-1, -1, 1}
    };

    result.phi2(d1, d4);
    result.phi2(d3, result.phi1(d1));
    result.phi2(d2, result.phi1(result.phi1(result.phi1(d1))));
    result.phi2(d5, result.phi1(result.phi1(d1)));
    result.phi2(d6, result.phi1(result.phi1(d5)));
    result.phi2(result.phi1(d5), result.phi1(result.phi1(result.phi1(d3))));
    result.phi2(result.phi1(result.phi1(result.phi1(d5))), result.phi1(d2));
    result.phi2(result.phi1(d6), result.phi1(result.phi1(d3)));
    result.phi2(result.phi1(result.phi1(result.phi1(d6))), result.phi1(result.phi1(d2)));
    result.phi2(result.phi1(result.phi1(d6)), result.phi1(result.phi1(d4)));
    result.phi2(result.phi1(result.phi1(result.phi1(d2))), result.phi1(d4));
    result.phi2(result.phi1(d3), result.phi1(result.phi1(result.phi1(d4))));

    return result;
}

int main()
{
    try {
        Device device{ 800, 800 };

        GCard c = cube();

        std::cout << "Euler characteristics: " << c.euler() << std::endl
            << "Number of vertices: " << c.vertices().size() << std::endl
            << "Number of edges: " << c.edges().size() << std::endl
            << "Number of faces: " << c.faces().size() << std::endl;

        std::cout << "Vertices: " << std::endl;
        for (auto vert : c.vertices()) {
            for (auto v : vert) {
                std::cout << v << ", ";
            }
            std::cout << std::endl;
        }

        std::cout << "Edges: " << std::endl;
        for (auto edge : c.edges()) {
            for (auto e : edge) {
                std::cout << e << ", ";
            }
            std::cout << std::endl;
        }

        std::cout << "Faces: " << std::endl;
        for (auto face : c.faces()) {
            for (auto f : face) {
                std::cout << f << ", ";
            }
            std::cout << std::endl;
        }

        // Try to draw object (WIP)
        /*GCardRenderer tore_renderer{tore, {
            { glm::vec3(-1, -1, 0) },
            { glm::vec3(1, -1, 0) },
            { glm::vec3(1, 1, 0) },
            { glm::vec3(-1, 1, 0) }
        }};
        tore_renderer.render();*/

        device.Run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    
    return 0;
}