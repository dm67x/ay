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

        // Pyramide
        GCard pyramide;
        Embedding pyramideEmbedding{ pyramide };
        Strand base, t1, t2, t3, t4;
        base = pyramide.newFace(4);
        t1 = pyramide.newFace(3);
        t2 = pyramide.newFace(3);
        t3 = pyramide.newFace(3);
        t4 = pyramide.newFace(3);

        pyramide.phi2(base, t1);
        pyramide.phi2(pyramide.phi1(base), t2);
        pyramide.phi2(pyramide.phi1(pyramide.phi1(base)), t3);
        pyramide.phi2(pyramide.phi1(pyramide.phi1(pyramide.phi1(base))), t4);
        pyramide.phi2(pyramide.phi1(t1), pyramide.phi1(pyramide.phi1(t4)));
        pyramide.phi2(pyramide.phi1(pyramide.phi1(t1)), pyramide.phi1(t2));
        pyramide.phi2(pyramide.phi1(t3), pyramide.phi1(pyramide.phi1(t2)));
        pyramide.phi2(pyramide.phi1(t4), pyramide.phi1(pyramide.phi1(t3)));

        pyramideEmbedding.reload();
        pyramideEmbedding[base] = { { -1, -1, -1} };
        pyramideEmbedding[pyramide.phi1(base)] = { {-1, 1, -1} };
        pyramideEmbedding[pyramide.phi1(pyramide.phi1(base))] = { {1, 1, -1} };
        pyramideEmbedding[pyramide.phi1(pyramide.phi1(pyramide.phi1(base)))] = { {1, -1, -1} };
        pyramideEmbedding[pyramide.phi1(pyramide.phi1(t1))] = { {0, 0, 1}, glm::vec3(0), glm::vec2(0), { 1, 0, 0, 1 } };

        GCardRenderer pyramideRenderer{ pyramide, pyramideEmbedding };

        glm::mat4 projection = glm::perspective(70.f, 
            static_cast<float>(device.width() / device.height()), 
            1.f, 100.f);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        
        while (device.run()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mainCamera.update(static_cast<float>(glfwGetTime()));

            glm::mat4 MVP = projection * mainCamera.view();

            baseProgram.use();
            baseProgram.uniform("MVP", [&](GLint l) {
                glUniformMatrix4fv(l, 1, GL_FALSE, &MVP[0][0]);
            });

            pyramideRenderer.render();

            baseProgram.reset();
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    
    return 0;
}