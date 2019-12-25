#include "application.hpp"
#include "utils/gltf_loader.hpp"
#include "entity/camera.hpp"
#include "rendering/shader_manager.hpp"
#include "rendering/shader.hpp"

#include <stdexcept>
#include <iostream>

int main(void)
{
    Application app;
    glTFLoader helmetLoader;
    Camera mainCamera{ "mainCamera", 1.f };
    ShaderManager shmgr;

    try {
        // Shader
        auto shader = shmgr.create("Standard");
        if (!shader->load("assets/shaders/phong.glsl")) {
            std::cerr << "cannot load shader" << std::endl;
        }

        // Camera
        mainCamera.backgroundColor(glm::vec4(1));
        mainCamera.position(0, 0, -2);
        float rotationAmount = 0;

        // Loader
        if (!helmetLoader.load("assets/models/DamagedHelmet.glb")) {
            throw std::exception("cannot load glTF file");
        }

        while (app.run()) {
            auto wsize = app.size();
            float width = static_cast<float>(wsize.first);
            float height = static_cast<float>(wsize.second);
            float ratio = width / height;

            if (width < height) {
                ratio = height / width;
            }

            mainCamera.aspectRatio(ratio);

            helmetLoader.rotateY(rotationAmount++);

            shmgr["Standard"]->use();
            mainCamera.draw(*shmgr["Standard"]);
            helmetLoader.draw(*shmgr["Standard"]);
            shmgr["Standard"]->reset();
        }
    }
    catch (const std::exception & ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}