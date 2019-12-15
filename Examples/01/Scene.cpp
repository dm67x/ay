#include "Scene.hpp"
#include "Scene/Manager.hpp"
#include "Render/GCard.hpp"
#include "Render/GCardRenderer.hpp"
#include "Render/Camera.hpp"
#include "Render/Embedding.hpp"
#include "render/Model.hpp"

MainScene::MainScene()
    : m_root { SceneManager::instance().getRoot()->create() }
{
    GCard* pyramide = new GCard;
    Embedding* pyramideEmbed = new Embedding{ *pyramide };
    GCardRenderer* pyramideRender = new GCardRenderer{ 
        *pyramide, *pyramideEmbed 
    };
    Camera* mainCamera = new Camera;

    Strand* base, *t1, *t2, *t3, *t4;
    base = pyramide->newFace(4);
    t1 = pyramide->newFace(3);
    t2 = pyramide->newFace(3);
    t3 = pyramide->newFace(3);
    t4 = pyramide->newFace(3);

    base->phi2 = t1;
    t1->phi2 = base;
    base->phi1->phi2 = t2;
    t2->phi2 = base->phi1;
    base->phi1->phi1->phi2 = t3;
    t3->phi2 = base->phi1->phi1;
    base->phi_1->phi2 = t4;
    t4->phi2 = base->phi_1;
    t1->phi1->phi2 = t4->phi1->phi1;
    t4->phi1->phi1->phi2 = t1->phi1;
    t1->phi1->phi1->phi2 = t2->phi1;
    t2->phi1->phi2 = t1->phi1->phi1;
    t3->phi1->phi2 = t2->phi1->phi1;
    t2->phi1->phi1->phi2 = t3->phi1;
    t4->phi1->phi2 = t3->phi1->phi1;
    t3->phi1->phi1->phi2 = t4->phi1;

    pyramideEmbed->reload();
    (*pyramideEmbed)[base] = { { -1, -1, -1} };
    (*pyramideEmbed)[base->phi1] = { {-1, 1, -1} };
    (*pyramideEmbed)[base->phi1->phi1] = { {1, 1, -1} };
    (*pyramideEmbed)[base->phi_1] = { {1, -1, -1} };
    (*pyramideEmbed)[t1->phi1->phi1] = {
        {0, 0, 1}, 
        glm::vec3(0), 
        glm::vec2(0), 
        { 1, 0, 0, 1 } 
    };

    pyramideRender->build();

    // Model
    /*Model* tore = new Model;
    tore->load("../Models/tore.obj");*/

    // Create scene nodes
    auto cameraNode = m_root->create();
    auto pyramideNode = m_root->create();
    //auto toreNode = m_root->create();

    // Attach objects
    cameraNode->attach(mainCamera);
    pyramideNode->attach(pyramideRender);
    //toreNode->attach(tore);
}