#include "window.hpp"
#include "context.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "scene.hpp"

int main(void)
{
    const int WIDTH = 1280;
    const int HEIGHT = 900;

    Window window(WIDTH, HEIGHT);
    Context* ctx = window.getContext();
    Scene scene(ctx, WIDTH, HEIGHT);

    ctx->shaderFromFile("blinn-phong", "../../assets/phong.vert.glsl", "../../assets/phong.frag.glsl");
    Mesh* mesh = Mesh::fromFile(ctx, "../../assets/xbox.glb");
    //Mesh* meshClone = mesh->clone();
    mesh->transform.scale = Vec3(.25f, .25f, .25f);
    mesh->transform.position.z = 3.f;

    scene.createPerspectiveCamera("mainCamera", 90.f, 0.1f, 100.f);
    scene.setMainCamera("mainCamera");
    Light* light = scene.createLight();
    light->position = Vec3(5.f, 1.f, 1.f);
    light->color = Color::red();
    light->power = 40.f;

    Light* light2 = scene.createLight();
    light2->position = Vec3(-5.f, 1.f, 1.f);
    light2->color = Color::green();
    light2->power = 45.f;

    scene.onRender = [&](Scene* scene, float deltaTime) {
        (void)scene;
        (void)deltaTime;

        mesh->transform.rotation.y += 1.f;
        mesh->render(0.f);
    };

    scene.onDestroy = [&](Scene* scene) {
        (void)scene;
        delete mesh;
    };

    while (window.isOpen()) {
        ctx->clear();
        ctx->viewport(0, 0, WIDTH, HEIGHT);
        ctx->shaderUse("blinn-phong");

        scene.render(0.f);
    }

    return 0;
}