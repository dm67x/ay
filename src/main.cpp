#include "window.hpp"
#include "context.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include <chrono>

int main(void)
{
    Window window(1280, 900);
    Context* ctx = window.getContext();
    Scene scene(ctx);

    ctx->shaderFromFile("blinn-phong", "../../assets/phong.vert.glsl", "../../assets/phong.frag.glsl");

    Mesh* mesh = Mesh::fromFile(ctx, "../../assets/cube.glb");
    mesh->transform.scale = Vec3(0.5f, 0.5f, 0.5f);
    mesh->transform.position.z = 5.f;

    scene.createFreeCamera("mainCamera", 90.f, 0.1f, 100.f);
    scene.setMainCamera("mainCamera");
    Light* light = scene.createDirectionalLight();
    light->position = Vec3(-3.f, 3.f, -3.f);
    light->color = Color::white();
    light->intensity = 8.f;

    scene.onRender = [&](Scene* scene, float deltaTime) {
        (void)scene;
        ctx->uiBegin();
        enum ImGuiWindowFlags_ flags = static_cast<enum ImGuiWindowFlags_>(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar);
        ctx->uiCreateWindow("Informations", [&] {
            std::stringstream fps;
            fps << "FPS: " << 1.f / deltaTime;
            ImGui::TextColored(ImVec4(1, 1, 0, 1), fps.str().c_str());
            ImGui::ColorEdit4("Light color", light->color.toPtr());
        }, flags);
        ctx->uiEnd();

        //mesh->transform.rotation.y += 100.f * deltaTime;
        mesh->render(deltaTime);
    };

    scene.onDestroy = [&](Scene* scene) {
        (void)scene;
        delete mesh;
    };

    auto start = std::chrono::steady_clock::now();

    while (window.isOpen()) {
        if (window.isKeyPressed(GLFW_KEY_B)) {
            mesh->setDebug(true);
        }

        if (window.isKeyPressed(GLFW_KEY_N)) {
            mesh->setDebug(false);
        }

        if (window.isKeyPressed(GLFW_KEY_ESCAPE)) {
            window.close();
        }

        const int WIDTH = window.getSize().first;
        const int HEIGHT = window.getSize().second;

        ctx->clear();
        ctx->viewport(0, 0, WIDTH, HEIGHT);
        ctx->shaderUse("blinn-phong");

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<float> diff = end - start;
        start = end;

        const float elapsedTime = diff.count();
        scene.render(elapsedTime);
    }

    return 0;
}