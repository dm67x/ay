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
        ctx->shaderFromFile("base", "../../assets/basic.vert.glsl", "../../assets/basic.frag.glsl");
        ctx->texture2DNew("albedo_wall", "../../assets/albedo_wall.png");
        Mesh* mesh = MeshFactory::fromFile(ctx, "../../assets/buddha.glb");
        mesh->transform.scale = Vec3(.25f, .25f, .25f);
        mesh->transform.position.z = 3.f;
        mesh->transform.rotation.x = -45.f;
        meshes.push_back(mesh);
    }

    ~MainScene() override {
        for (auto mesh : meshes) {
            delete mesh;
        }
    }

    void update(float deltaTime) override {
        (void)deltaTime;
        angle += 1.f;
    }

    void render(float deltaTime) override {
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

    while (window.isOpen()) {
        scene.update(0.f);
        scene.render(0.f);
    }

    return 0;
}