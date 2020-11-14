#include "window.hpp"
#include "context.hpp"
#include "math.hpp"
#include "mesh_factory.hpp"
#include "camera.hpp"

int main(void)
{
    Window window(1280, 900);
    Context* ctx = window.getContext();
    ctx->shaderFromFile("base", "../../assets/basic.vert.glsl", "../../assets/basic.frag.glsl");

    ctx->texture2DNew("albedo_wall", "../../assets/albedo_wall.png");

    std::vector<Mesh*> models;

    for (int i = 0; i < 10; i++) {
        Mesh* model = MeshFactory::cube(ctx);
        model->transform.scale = Vec3(0.5f, 0.5f, 0.5f);
        model->transform.position.z = 3.f;
        model->transform.rotation.x = -45.f;
        model->transform.position.x = static_cast<float>(i * 2 - 5);
        models.push_back(model);
    }

    float angle = 0.f;

    Camera* mainCamera = new PerspectiveCamera(ctx, 90.f, 1280.f / 900.f, 0.1f, 100.f);

    while (window.isOpen()) {
        angle += 1.f;

        glEnable(GL_DEPTH_TEST);

        auto windowSize = window.getSize();

        ctx->clear();
        ctx->viewport(0, 0, windowSize.first, windowSize.second);
        ctx->shaderUse("base");

        mainCamera->setAspectRatio(static_cast<float>(windowSize.first) / static_cast<float>(windowSize.second));
        mainCamera->update(0.f);

        auto albedo = ctx->texture2DGet("albedo_wall");
        if (albedo) {
            albedo->use();
        }
        ctx->shaderUniform("albedo", 0);

        for (auto model : models) {
            model->transform.rotation.y = angle;
            model->render(0.f);
        }
    }

    delete mainCamera;
    for (auto model : models) {
        delete model;
    }
    return 0;
}