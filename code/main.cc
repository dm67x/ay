#include "device.h"
#include "scene/manager.h"
#include "render/camera.h"
#include "render/model.h"
#include "render/shader.h"
#include "render/shader_manager.h"
#include "render/light.h"

#include <stdexcept>
#include <iostream>
#include <glm/gtx/transform.hpp>

int main(int argc, char** argv)
{
    /*if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <model>" << std::endl;
        return 1;
    }*/

    try {
        Device device{ 800, 800 };
        ShaderManager shmgr;
        SceneManager smgr;

        auto mainScene = smgr.getRoot().create();

        // Shader
        shmgr.create("Standard");
        shmgr["Standard"]->vertex().fromFile("shaders/phong_vert.glsl");
        shmgr["Standard"]->fragment().fromFile("shaders/phong_frag.glsl");
        shmgr["Standard"]->build();

        // Node
        auto cameraNode = mainScene->create();
        auto lightNode = mainScene->create();
        auto objectNode = mainScene->create();

        // Camera
        auto mainCamera = new Camera;
        mainCamera->translate(glm::vec3(0, 0, -10));
        mainCamera->target(glm::vec3(0));
        mainCamera->rotate(glm::radians(20.f), glm::vec3(1, 0, 0));
        cameraNode->attach(mainCamera);

        // object
        Model object;
        if (object.load("models/dodge")) {
            objectNode->attach(&object);
        }

        // Light
        Light* light = new Light;
        light->position = glm::vec3(1, 1, 1);
        light->power = 5.f;
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
            
            object.rotate(glm::radians(rotationAmount++), glm::vec3(0, 1, 0));
            shmgr["Standard"]->use();
            shmgr["Standard"]->uniform("projectionMatrix", projection);
            smgr.render(*shmgr["Standard"]);
            shmgr["Standard"]->reset();
        }
    }
    catch (const std::exception & e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}