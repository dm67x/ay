#include "Device.hpp"
#include "Render/Material.hpp"
#include "Scene/Manager.hpp"
#include "Scene.hpp"
#include "Render/Camera.hpp"

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
        Shader<GL_VERTEX_SHADER> vertex;
        Shader<GL_FRAGMENT_SHADER> fragment;
        vertex.fromFile("../shaders/base_vert.glsl");
        fragment.fromFile("../shaders/base_frag.glsl");

        Material baseMaterial{ vertex, fragment };

        auto cameraNode = (*mainScene.getNode())[0];
        Camera* mainCamera = dynamic_cast<Camera*>(cameraNode->entity());
        if (!mainCamera)
            throw std::exception("error camera not created");

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