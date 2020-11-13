#include "window.hpp"
#include "context.hpp"
#include "math.hpp"
#include "mesh_factory.hpp"

int main(void)
{
    Window window(1280, 900);
    Context* ctx = window.getContext();
    ctx->shaderFromFile("base", "../../assets/basic.vert.glsl", "../../assets/basic.frag.glsl");

    Mesh* model = MeshFactory::fromFile(ctx, "../../assets/suzanne.obj");
    ctx->texture2DNew("logo", "../../assets/logo.png");

    model->transform.scale = Vec3(0.5f, 0.5f, 0.5f);
    float angle = 0.f;

    while (window.isOpen()) {
        angle += 10.f;
        model->transform.rotation.x = radians(angle);
        model->transform.rotation.y = radians(angle);

        ctx->clear();
        ctx->viewport(0, 0, 1280, 900);
        ctx->shaderUse("base");
        ctx->texture2DGet("logo")->use();
        ctx->shaderUniform("logoTexture", 0);
        model->render(0.f);
    }

    delete model;
    return 0;
}