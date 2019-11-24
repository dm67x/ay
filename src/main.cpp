#include "device.h"
#include "gcard.h"
#include "gcard_renderer.h"
#include "embedding.h"
#include "shader.h"
#include "camera.h"

#include <stdexcept>
#include <iostream>
#include <glm/gtx/transform.hpp>

int main()
{
    try {
        Device device{ 800, 800 };
        Camera mainCamera;

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
            { glm::vec3(-1, 1, -1), glm::vec3(0), glm::vec2(0), glm::vec4(0.5) },
            { glm::vec3(1, 1, -1), glm::vec3(0), glm::vec2(0), glm::vec4(0.5) },
            { glm::vec3(1, -1, -1), glm::vec3(0), glm::vec2(0), glm::vec4(0.5) },
            { glm::vec3(-1, -1, -1), glm::vec3(0), glm::vec2(0), glm::vec4(0.5) },
            { glm::vec3(-1, 1, 1), glm::vec3(0), glm::vec2(0), glm::vec4(0.5) },
            { glm::vec3(1, 1, 1), glm::vec3(0), glm::vec2(0), glm::vec4(0.5) },
            { glm::vec3(1, -1, 1), glm::vec3(0), glm::vec2(0), glm::vec4(0.5) },
            { glm::vec3(-1, -1, 1), glm::vec3(0), glm::vec2(0), glm::vec4(0.5) }
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

        // Shader
        Shader baseVertexShader{ GL_VERTEX_SHADER };
        Shader baseFragmentShader{ GL_FRAGMENT_SHADER };
        ShaderProgram baseProgram{ baseVertexShader, baseFragmentShader };

        baseVertexShader.fromMemory(
            "#version 330\n"
            "layout(location = 0) in vec3 position;\n"
            "layout(location = 1) in vec3 normal;\n"
            "layout(location = 2) in vec2 uv;\n"
            "layout(location = 3) in vec4 color;\n"
            "uniform mat4 MVP;\n"
            "out vec4 colorOut;\n"
            "void main() {\n"
            "gl_Position = MVP * vec4(position, 1.0);\n"
            "colorOut = color;\n"
            "}"
        );

        baseFragmentShader.fromMemory(
            "#version 330\n"
            "in vec4 colorOut;\n"
            "out vec4 fragOut;\n"
            "void main() {\n"
            "fragOut = colorOut;\n"
            "}"
        );

        baseProgram.build();

        glm::mat4 projection = glm::perspective(70.f, 1.f, 0.f, 100.f);
        
        while (device.run()) {
            glClear(GL_COLOR_BUFFER_BIT);

            mainCamera.update(static_cast<float>(glfwGetTime()));

            glm::mat4 MVP = projection * mainCamera.view();

            baseProgram.use();
            baseProgram.uniform("MVP", [&](GLint l) {
                glUniformMatrix4fv(l, 1, GL_FALSE, &MVP[0][0]);
            });
            cubeRenderer.render();
            baseProgram.reset();
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    
    return 0;
}