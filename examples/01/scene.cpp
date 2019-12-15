#include "Scene.hpp"
#include "Scene/Manager.hpp"
#include "Render/GCard.hpp"
#include "Render/GCardRenderer.hpp"
#include "Render/Camera.hpp"
#include "Render/Embedding.hpp"

MainScene::MainScene()
    : m_root { SceneManager::instance().getRoot()->create() }
{
    GCard* pyramide = new GCard;
    Embedding* pyramideEmbed = new Embedding{ *pyramide };
    GCardRenderer* pyramideRender = new GCardRenderer{ 
        *pyramide, *pyramideEmbed 
    };
    Camera* mainCamera = new Camera;

    Strand base, t1, t2, t3, t4;
    base = pyramide->newFace(4);
    t1 = pyramide->newFace(3);
    t2 = pyramide->newFace(3);
    t3 = pyramide->newFace(3);
    t4 = pyramide->newFace(3);

    pyramide->phi2(base, t1);
    pyramide->phi2(pyramide->phi1(base), t2);
    pyramide->phi2(pyramide->phi1(pyramide->phi1(base)), t3);
    pyramide->phi2(pyramide->phi_1(base), t4);
    pyramide->phi2(pyramide->phi1(t1), pyramide->phi1(pyramide->phi1(t4)));
    pyramide->phi2(pyramide->phi1(pyramide->phi1(t1)), pyramide->phi1(t2));
    pyramide->phi2(pyramide->phi1(t3), pyramide->phi1(pyramide->phi1(t2)));
    pyramide->phi2(pyramide->phi1(t4), pyramide->phi1(pyramide->phi1(t3)));

    pyramideEmbed->reload();
    (*pyramideEmbed)[base] = { { -1, -1, -1} };
    (*pyramideEmbed)[pyramide->phi1(base)] = { {-1, 1, -1} };
    (*pyramideEmbed)[pyramide->phi1(pyramide->phi1(base))] = { {1, 1, -1} };
    (*pyramideEmbed)[pyramide->phi_1(base)] = { {1, -1, -1} };
    (*pyramideEmbed)[pyramide->phi1(pyramide->phi1(t1))] =
        { {0, 0, 1}, glm::vec3(0), glm::vec2(0), { 1, 0, 0, 1 } };

    pyramideRender->build();

    // Create scene nodes
    auto cameraNode = m_root->create();
    auto pyramideNode = m_root->create();

    // Attach objects
    cameraNode->attach(mainCamera);
    pyramideNode->attach(pyramideRender);
}