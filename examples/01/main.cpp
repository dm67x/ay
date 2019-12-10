#include "render/device.h"
#include "render/material.h"
#include "scene/manager.h"
#include "scene.h"
#include "render/camera.h"

#include <stdexcept>
#include <iostream>
#include <glm/gtx/transform.hpp>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    try {
        Device device{ 800, 800 };
        MainScene mainScene;

        // Base material
        Material baseMaterial{ 
            "../shaders/base_vert.glsl", 
            "../shaders/base_frag.glsl" 
        };

        // get camera
        SceneNode* cameraNode = (*mainScene.getNode())[0];
        Camera* mainCamera = dynamic_cast<Camera*>(cameraNode->entity());
        if (!mainCamera) {
            throw std::runtime_error("not a camera");
        }

        // run
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

            mainCamera->update(device.systemClock().elapsedTime() * 1000.0);

            glm::mat4 MVP = projection * mainCamera->view();

            baseMaterial.use();
            baseMaterial.program().uniform("MVP", MVP);

            SceneManager::instance().render();

            baseMaterial.reset();
        }
    }
    catch (const std::exception & e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}