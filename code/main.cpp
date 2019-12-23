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
    glTFLoader loader;
    Camera mainCamera{ "mainCamera", 1.f };
    ShaderManager shmgr;

    try {
        // Shader
        auto shader = shmgr.create("Standard");
        shader->vertex().fromFile("../shaders/phong_vert.glsl");
        shader->fragment().fromFile("../shaders/phong_frag.glsl");
        shader->build();

        // Camera
        mainCamera.translate(glm::vec3(0, 0, -2));
        float rotationAmount = 0;

        // Loader
        if (!loader.load("../models/DamagedHelmet.glb")) {
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
            mainCamera.rotate(glm::radians(rotationAmount++), glm::vec3(0, 1, 0));

            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shmgr["Standard"]->use();
            mainCamera.draw(*shmgr["Standard"]);
            loader.draw(*shmgr["Standard"]);
            shmgr["Standard"]->reset();
        }
    }
    catch (const std::exception & ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}