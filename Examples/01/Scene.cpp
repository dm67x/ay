#include "Scene.hpp"
#include "Scene/Manager.hpp"
#include "Render/Camera.hpp"
#include "Render/Mesh.hpp"
#include "Render/MeshRenderer.hpp"

MainScene::MainScene()
    : m_root { SceneManager::instance().getRoot()->create() }
{
    auto cameraNode = m_root->create();
    auto meshNode = m_root->create();

    Camera* mainCamera = new Camera;
    mainCamera->translate(glm::vec3(0, 1, -5));
    cameraNode->attach(mainCamera);

    Mesh* mesh = new Mesh;
    mesh->load("../Models/tore.obj");
    MeshRenderer* renderer = new MeshRenderer{ *mesh };
    renderer->build();
    meshNode->attach(renderer);
}