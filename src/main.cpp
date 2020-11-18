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

    Mesh* mesh = Mesh::fromFile(ctx, "../../assets/buddha.glb");
    Mesh* plane = Mesh::plane(ctx);

    mesh->transform.scale = glm::vec3(1.5f);
    mesh->transform.position.z = -5.f;
    mesh->transform.position.y = 0.f;
    mesh->transform.rotation.x = glm::radians(90.f);

    plane->transform.position.z = -5.f;
    plane->transform.position.y = -.8f;
    plane->transform.scale = glm::vec3(10.f);
    plane->transform.rotation.x = glm::radians(90.f);
    plane->transform.rotation.z = glm::radians(180.f);

    scene.createFreeCamera("mainCamera", 90.f, 0.1f, 100.f);
    scene.setMainCamera("mainCamera");

    Light* light = scene.createPointLight();
    light->position = glm::vec3(0.f, 3.f, -5.f);
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

        mesh->transform.rotation.y += glm::radians(100.f * deltaTime);
        mesh->render(deltaTime);
        plane->render(deltaTime);
    };

    scene.onDestroy = [&](Scene* scene) {
        (void)scene;
        delete mesh;
        delete plane;
    };

    auto start = std::chrono::steady_clock::now();

    while (window.isOpen()) {
        if (window.isKeyPressed(GLFW_KEY_B)) {
            mesh->setDebug(true);
        }

        if (window.isKeyPressed(GLFW_KEY_N)) {
            mesh->setDebug(false);
        }

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