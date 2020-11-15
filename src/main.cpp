#include "window.hpp"
#include "context.hpp"
#include "mesh_factory.hpp"
#include "camera.hpp"
#include "scene.hpp"

struct MainScene : public Scene {
    float angle;
    std::vector<Mesh*> meshes;

    MainScene(Context* ctx, int width, int height)
        : Scene(ctx, width, height), angle(0.f), meshes()
    {
        ctx->shaderFromFile("base", "../../assets/phong.vert.glsl", "../../assets/phong.frag.glsl");
        ctx->texture2DNew("albedo_wall", "../../assets/albedo_wall.png");
        Mesh* mesh = MeshFactory::fromFile(ctx, "../../assets/buddha.glb");
        mesh->transform.scale = Vec3(2.f, 2.f, 2.f);
        mesh->transform.position.z = 2.f;
        mesh->transform.rotation.x = 90.f;
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

        auto albedo = ctx->texture2DGet("albedo_wall");
        if (albedo) {
            albedo->use();
        }
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
    light->position = Vec3(1.f, 1.f, 1.f);
    light->color = Color::white();
    light->power = 40.f;

    Light* light2 = scene.createLight();
    light2->position = Vec3(-1.f, 1.f, 1.f);
    light2->color = Color::yellow();
    light2->power = 45.f;

    while (window.isOpen()) {
        scene.render(0.f);
    }

    return 0;
}