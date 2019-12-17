#include "Device.hpp"
#include "Scene/Manager.hpp"
#include "Render/Camera.hpp"
#include "Render/Model.hpp"
#include "Render/Shader.hpp"
#include "Render/Light.hpp"

#include <stdexcept>
#include <iostream>
#include <glm/gtx/transform.hpp>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    try {
        Device device{ 800, 800 };
        auto mainScene = SceneManager::instance().getRoot()->create();

        // Shader
        Shader<GL_VERTEX_SHADER> vertex;
        Shader<GL_FRAGMENT_SHADER> fragment;
        ShaderProgram program{ vertex, fragment };

        vertex.fromFile("../shaders/phong_vert.glsl");
        fragment.fromFile("../shaders/phong_frag.glsl");
        program.build();

        // Node
        auto cameraNode = mainScene->create();
        auto lightNode = mainScene->create();
        auto dodgeNode = mainScene->create();

        // Camera
        auto mainCamera = new Camera;
        mainCamera->translate(glm::vec3(0, 0, -10));
        mainCamera->target(glm::vec3(0));
        mainCamera->rotate(glm::radians(20.f), glm::vec3(1, 0, 0));
        cameraNode->attach(mainCamera);

        // Dodge challenger
        Model dodge;
        if (dodge.load("../Models/dodge")) {
            dodgeNode->attach(&dodge);
        }

        // Light
        Light* light = new Light;
        light->position = glm::vec3(0, 10, 0);
        lightNode->attach(light);

        float rotationAmount = 0;

        // run
        while (device.run()) {
            device.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
            
            dodge.rotate(glm::radians(rotationAmount++), glm::vec3(0, 1, 0));
            program.use();
            program.uniform("projectionMatrix", projection);
            SceneManager::instance().render(program);
            program.reset();
        }
    }
    catch (const std::exception & e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}