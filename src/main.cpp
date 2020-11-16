#include "window.hpp"
#include "context.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include <chrono>

int main(void)
{
    const int WIDTH = 1280;
    const int HEIGHT = 900;

    Window window(WIDTH, HEIGHT);
    Context* ctx = window.getContext();
    Scene scene(ctx, WIDTH, HEIGHT);

    ctx->shaderFromFile("blinn-phong", "../../assets/phong.vert.glsl", "../../assets/phong.frag.glsl");

    Mesh* mesh = Mesh::fromFile(ctx, "../../assets/scene.glb");
    mesh->transform.scale = Vec3(0.5f, 0.5f, 0.5f);
    mesh->transform.position.z = 5.f;

    scene.createPerspectiveCamera("mainCamera", 90.f, 0.1f, 100.f);
    scene.setMainCamera("mainCamera");
    Light* light = scene.createLight();
    light->position = Vec3(0.f, 0.f, -2.f);
    light->color = Color::white();
    light->power = 80.f;

    scene.onRender = [&](Scene* scene, float deltaTime) {
        (void)scene;
        mesh->transform.rotation.y += 100.f * deltaTime;
        mesh->render(deltaTime);
    };

    scene.onDestroy = [&](Scene* scene) {
        (void)scene;
        delete mesh;
    };

    auto start = std::chrono::steady_clock::now();

    while (window.isOpen()) {
        ctx->clear();
        ctx->viewport(0, 0, WIDTH, HEIGHT);
        ctx->shaderUse("blinn-phong");

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<float> diff = end - start;
        start = end;

        const float elapsedTime = diff.count();
        spdlog::info("FPS: {}", 1.f / elapsedTime);

        scene.render(elapsedTime);
    }

    return 0;
}