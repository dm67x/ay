#include "ay.hpp"
#include <chrono>

using namespace ay;

void renderScene(Scene* scene, f32 deltaTime) {
    auto ctx = scene->getContext();
    static f32 angle;
    
    ctx->uiBegin();
    enum ImGuiWindowFlags_ flags = static_cast<enum ImGuiWindowFlags_>(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar);
    ctx->uiCreateWindow("Informations", [&] {
        std::stringstream fps;
        fps << "FPS: " << 1.f / deltaTime;
        ImGui::TextColored(ImVec4(1, 1, 0, 1), fps.str().c_str());
    }, flags);
    ctx->uiEnd();

    angle += deltaTime * 100.f;
    auto model = scene->getModel("Duck");
    model->transform.rotation = glm::quat(glm::vec3(0.f, glm::radians(angle), 0.f));
    model->render(deltaTime);

    model->transform.position.x = 3.f;
    model->transform.rotation = glm::quat(glm::vec3(0));
    model->render(deltaTime);
}

void destroyScene(Scene* scene) {
    
}

int main(void)
{
    Window window(1280, 900);
    Context* ctx = window.getContext();
    Scene scene(ctx);

    Model* model = scene.createModel("Duck", "../assets/Duck.glb");
    model->transform.position.z = 5.f;
    model->transform.scale = glm::vec3(.5f);

    scene.createFreeCamera("mainCamera", 90.f, 0.1f, 100.f);
    scene.setMainCamera("mainCamera");

    Light* light = scene.createPointLight();
    light->position = glm::vec3(0.f, 5.f, 5.f);
    light->color = Color::white();
    light->intensity = 8.f;

    scene.onRender = renderScene;
    scene.onDestroy = destroyScene;

    auto start = std::chrono::steady_clock::now();

    while (window.isOpen()) {
        const i32 WIDTH = window.getSize().first;
        const i32 HEIGHT = window.getSize().second;

        ctx->clear();
        ctx->viewport(0, 0, WIDTH, HEIGHT);

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<f32> diff = end - start;
        start = end;

        const f32 elapsedTime = diff.count();
        scene.render(elapsedTime);
    }

    return 0;
}