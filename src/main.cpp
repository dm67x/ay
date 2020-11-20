#include "window.hpp"
#include "context.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include <chrono>

int main(void)
{
    Window window(1280, 900);
    Context* ctx = window.getContext();
    Scene scene(ctx);

    Model* model = Model::fromFile(ctx, "../assets/Duck.glb");

    model->transform.position.z = 5.f;
    model->transform.scale = glm::vec3(.5f);

    scene.createFreeCamera("mainCamera", 90.f, 0.1f, 100.f);
    scene.setMainCamera("mainCamera");

    Light* light = scene.createDirectionalLight();
    light->position = glm::vec3(0.f, 5.f, 5.f);
    light->color = Color::white();
    light->intensity = 8.f;

    float angle = 0.f;

    scene.onRender = [&](Scene* scene, float deltaTime) {
        (void)scene;
        ctx->uiBegin();
        enum ImGuiWindowFlags_ flags = static_cast<enum ImGuiWindowFlags_>(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar);
        ctx->uiCreateWindow("Informations", [&] {
            std::stringstream fps;
            fps << "FPS: " << 1.f / deltaTime;
            ImGui::TextColored(ImVec4(1, 1, 0, 1), fps.str().c_str());
        }, flags);
        ctx->uiEnd();

        angle += deltaTime * 100.f;

        model->transform.rotation = glm::quat(glm::vec3(0.f, glm::radians(angle), 0.f));
        model->render(deltaTime);
    };

    scene.onDestroy = [&](Scene* scene) {
        (void)scene;
        delete model;
    };

    auto start = std::chrono::steady_clock::now();

    while (window.isOpen()) {
        const int WIDTH = window.getSize().first;
        const int HEIGHT = window.getSize().second;

        ctx->clear();
        ctx->viewport(0, 0, WIDTH, HEIGHT);

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<float> diff = end - start;
        start = end;

        const float elapsedTime = diff.count();
        scene.render(elapsedTime);
    }

    return 0;
}