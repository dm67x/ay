#include "device.hpp"
#include "scene/scene.hpp"
#include "shader/shader_manager.hpp"
#include "shader/shader.hpp"
#include "entity/camera.hpp"

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
        Scene mainScene{ "main" };
        ShaderManager shmgr;
        Camera mainCamera;

        // Shader
        auto shader = shmgr.create("Standard");
        shader->vertex().fromFile("shaders/phong_vert.glsl");
        shader->fragment().fromFile("shaders/phong_frag.glsl");
        shader->build();

        // Scene
        if (!mainScene.load("models/cube.glb")) {
            std::cout << "cannot load scene" << std::endl;
        }

        // Camera
        mainCamera.translate(glm::vec3(0, 0, -10));
        mainCamera.target(glm::vec3(0));
        mainCamera.rotate(glm::radians(20.f), glm::vec3(1, 0, 0));

        float rotationAmount = 0;

        // run
        while (device.run()) {
            device.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto wsize = device.size();
            float width = static_cast<float>(wsize.first);
            float height = static_cast<float>(wsize.second);
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

            mainCamera.projection(projection);

            shmgr["Standard"]->use();
            mainCamera.draw(*shmgr["Standard"]);
            mainScene.draw(*shmgr["Standard"]);
            shmgr["Standard"]->reset();
        }
    }
    catch (const std::exception & e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}