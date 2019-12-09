#include "render/device.h"
#include "render/gcard.h"
#include "render/gcard_renderer.h"
#include "render/embedding.h"
#include "render/shader.h"
#include "render/camera.h"
#include "scene/manager.h"

#include <stdexcept>
#include <iostream>
#include <glm/gtx/transform.hpp>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    try {
        Device device{ 800, 800 };

        // Shader
        Shader baseVertexShader{ GL_VERTEX_SHADER };
        Shader baseFragmentShader{ GL_FRAGMENT_SHADER };
        ShaderProgram baseProgram{ baseVertexShader, baseFragmentShader };

        baseVertexShader.fromFile("../shaders/base_vert.glsl");
        baseFragmentShader.fromFile("../shaders/base_frag.glsl");
        baseProgram.build();

        // Create objects (pyramide)
        GCard pyramide;
        Embedding pyramideEmbed{ pyramide };

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

        pyramideEmbed.reload();
        pyramideEmbed[base] = { { -1, -1, -1} };
        pyramideEmbed[pyramide.phi1(base)] = { {-1, 1, -1} };
        pyramideEmbed[pyramide.phi1(pyramide.phi1(base))] = { {1, 1, -1} };
        pyramideEmbed[pyramide.phi1(pyramide.phi1(pyramide.phi1(base)))] =
        { {1, -1, -1} };
        pyramideEmbed[pyramide.phi1(pyramide.phi1(t1))] =
        { {0, 0, 1}, glm::vec3(0), glm::vec2(0), { 1, 0, 0, 1 } };

        GCardRenderer pyramideRender{ pyramide, pyramideEmbed };

        // Camera
        Camera mainCamera;

        // Create scene nodes
        auto cameraNode = SceneManager::instance().getRoot()->create();
        auto pyramideNode =
            SceneManager::instance().getRoot()->create();

        // Attach objects
        cameraNode->attach(&mainCamera);
        pyramideNode->attach(&pyramideRender);

        while (device.run()) {
            float width = static_cast<float>(device.width());
            float height = static_cast<float>(device.height());
            glm::mat4 projection = glm::mat4();

            if (width > height) {
                projection = glm::perspective(70.f,
                    width / height,
                    1.f, 50.f);
            }
            else {
                projection = glm::perspective(70.f,
                    height / width,
                    1.f, 50.f);
            }

            device.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mainCamera.update(device.systemClock().elapsedTime() * 1000.0);

            glm::mat4 MVP = projection * mainCamera.view();

            baseProgram.use();
            baseProgram.uniform("MVP", MVP);

            SceneManager::instance().render();

            baseProgram.reset();
        }
    }
    catch (const std::exception & e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}