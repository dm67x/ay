#include "Device.hpp"
#include "Scene/Manager.hpp"
#include "Render/Camera.hpp"
#include "Render/MeshRenderer.hpp"
#include "Render/Material.hpp"
#include "Render/Shader.hpp"
#include "Render/Mesh.hpp"
#include "Render/MaterialManager.hpp"

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

        // Add materials
        Material toreMat{ "tore" };
        if (toreMat.load("../Models/tore.mtl")) {
            MaterialManager::instance().add(toreMat);
        }

        Material suzanneMat{ "suzanne" };
        if (suzanneMat.load("../Models/suzanne.mtl")) {
            MaterialManager::instance().add(suzanneMat);
        }

        // Shader
        Shader<GL_VERTEX_SHADER> vertex;
        Shader<GL_FRAGMENT_SHADER> fragment;
        ShaderProgram program{ vertex, fragment };

        vertex.fromFile("../shaders/phong_vert.glsl");
        fragment.fromFile("../shaders/phong_frag.glsl");
        program.build();

        // Node
        auto cameraNode = mainScene->create();
        auto toreNode = mainScene->create();
        auto suzanneNode = mainScene->create();

        // Camera
        auto mainCamera = new Camera;
        mainCamera->translate(glm::vec3(0, 0, -5));
        mainCamera->target(glm::vec3(0));
        mainCamera->rotate(glm::radians(20.f), glm::vec3(1, 0, 0));
        cameraNode->attach(mainCamera);

        // Tore
        Mesh* toreMesh = new Mesh;
        toreMesh->load("../Models/tore.obj");
        MeshRenderer* toreRenderer = new MeshRenderer{ *toreMesh, "tore" };
        toreRenderer->build();
        toreNode->attach(toreRenderer);

        // Suzanne
        Mesh* suzanneMesh = new Mesh;
        suzanneMesh->load("../Models/suzanne.obj");
        MeshRenderer* suzanneRenderer = new MeshRenderer{ *suzanneMesh, "suzanne" };
        suzanneRenderer->build();
        suzanneNode->attach(suzanneRenderer);
        suzanneRenderer->translate(glm::vec3(0, 0.5f, 0));
        suzanneRenderer->rotate(glm::radians(-45.f), glm::vec3(1, 0, 0));

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

            
            suzanneRenderer->rotate(glm::radians(rotationAmount++), glm::vec3(0, 1, 0));
            toreRenderer->rotate(glm::radians(rotationAmount++), glm::vec3(0, 1, 0));

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