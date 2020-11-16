#include "window.hpp"
#include "context.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "scene.hpp"

struct MainScene : public Scene {
    float angle;
    std::vector<Mesh*> meshes;

    MainScene(Context* ctx, int width, int height)
        : Scene(ctx, width, height), angle(0.f), meshes()
    {
        ctx->shaderFromFile("base", "../../assets/phong.vert.glsl", "../../assets/phong.frag.glsl");
        Mesh* mesh = Mesh::fromFile(ctx, "../../assets/xbox.glb");
        mesh->transform.scale = Vec3(.25f, .25f, .25f);
        mesh->transform.position.z = 3.f;
        meshes.push_back(mesh);
    }

    ~MainScene() override {
        for (auto mesh : meshes) {
            delete mesh;
        }
    }

    void render(float deltaTime) override {
        angle += 1.f;

        ctx->clear();
        ctx->viewport(0, 0, width, height);
        ctx->shaderUse("base");

        Scene::render(deltaTime);

        ctx->shaderUniform("albedo", 0);

        for (auto mesh : meshes) {
            mesh->transform.rotation.y = angle;
            mesh->render(0.f);
        }
    }
};

int main(void)
{
    Window window(1280, 900);
    Context* ctx = window.getContext();
    MainScene scene(ctx, 1280, 900);
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

    while (window.isOpen()) {
        scene.render(0.f);
    }

    return 0;
}