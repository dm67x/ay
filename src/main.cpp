#include "device.h"
#include "gcard.h"
#include "gcard_renderer.h"
#include "embedding.h"

#include <stdexcept>
#include <iostream>

int main()
{
    try {
        Device device{ 800, 800 };

        GCard cube;
        Strand d1, d2, d3, d4, d5, d6;
        Embedding cubeEmbedding(cube);

        d1 = cube.newFace(4);
        d2 = cube.newFace(4);
        d3 = cube.newFace(4);
        d4 = cube.newFace(4);
        d5 = cube.newFace(4);
        d6 = cube.newFace(4);

        Vertex P[8] = {
            { glm::vec3(-1, 1, -1) },
            { glm::vec3(1, 1, -1) },
            { glm::vec3(1, -1, -1) },
            { glm::vec3(-1, -1, -1) },
            { glm::vec3(-1, 1, 1) },
            { glm::vec3(1, 1, 1) },
            { glm::vec3(1, -1, 1) },
            { glm::vec3(-1, -1, 1) }
        };

        cube.phi2(d1, d4);
        cube.phi2(d3, cube.phi1(d1));
        cube.phi2(d2, cube.phi1(cube.phi1(cube.phi1(d1))));
        cube.phi2(d5, cube.phi1(cube.phi1(d1)));
        cube.phi2(d6, cube.phi1(cube.phi1(d5)));
        cube.phi2(cube.phi1(d5), cube.phi1(cube.phi1(cube.phi1(d3))));
        cube.phi2(cube.phi1(cube.phi1(cube.phi1(d5))), cube.phi1(d2));
        cube.phi2(cube.phi1(d6), cube.phi1(cube.phi1(d3)));
        cube.phi2(cube.phi1(cube.phi1(cube.phi1(d6))), cube.phi1(cube.phi1(d2)));
        cube.phi2(cube.phi1(cube.phi1(d6)), cube.phi1(cube.phi1(d4)));
        cube.phi2(cube.phi1(cube.phi1(cube.phi1(d2))), cube.phi1(d4));
        cube.phi2(cube.phi1(d3), cube.phi1(cube.phi1(cube.phi1(d4))));

        cubeEmbedding.reload(); // To update list of vertices
        // Embed
        cubeEmbedding[d1] = P[2];
        cubeEmbedding[cube.phi1(d1)] = P[3];
        cubeEmbedding[cube.phi1(cube.phi1(d1))] = P[0];
        cubeEmbedding[cube.phi1(cube.phi1(cube.phi1(d1)))] = P[1];
        cubeEmbedding[cube.phi1(cube.phi1(d3))] = P[7];
        cubeEmbedding[cube.phi1(cube.phi1(cube.phi1(d3)))] = P[4];
        cubeEmbedding[cube.phi1(cube.phi1(cube.phi1(d2)))] = P[6];
        cubeEmbedding[cube.phi1(cube.phi1(d2))] = P[5];

        std::cout << "Euler characteristics: " << cube.euler() << std::endl
            << "Number of vertices: " << cube.vertices().size() << std::endl
            << "Number of edges: " << cube.edges().size() << std::endl
            << "Number of faces: " << cube.faces().size() << std::endl;

        std::cout << "Vertices: " << std::endl;
        for (auto vert : cube.vertices()) {
            for (auto v : vert) {
                std::cout << v << ", ";
            }
            std::cout << std::endl;
        }

        std::cout << "Edges: " << std::endl;
        for (auto edge : cube.edges()) {
            for (auto e : edge) {
                std::cout << e << ", ";
            }
            std::cout << std::endl;
        }

        std::cout << "Faces: " << std::endl;
        for (auto face : cube.faces()) {
            for (auto f : face) {
                std::cout << f << ", ";
            }
            std::cout << std::endl;
        }

        // DOT file
        cube.graph("cube_graph.dot");

        // Draw object
        GCardRenderer cubeRenderer{cube, cubeEmbedding};
        
        while (device.Run()) {
            cubeRenderer.render();
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    
    return 0;
}