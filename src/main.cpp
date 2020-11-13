#include "window.hpp"
#include "context.hpp"
#include "math.hpp"
#include "mesh_factory.hpp"

int main(void)
{
    Window window(1280, 900);
    Context* ctx = window.getContext();
    ctx->shaderFromFile("base", "../../assets/basic.vert.glsl", "../../assets/basic.frag.glsl");

    Mesh cube = MeshFactory::plane(ctx);
    ctx->texture2DNew("logo", "../../assets/logo.png");

    while (window.isOpen()) {
        cube.update(0.f);

        ctx->clear();
        ctx->viewport(0, 0, 1280, 900);
        ctx->shaderUse("base");
        ctx->texture2DGet("logo")->use();
        ctx->shaderUniform("logoTexture", 0);
        cube.render(0.f);
    }

    return 0;
}