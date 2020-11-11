#include "window.hpp"
#include "context.hpp"
#include "math.hpp"

int main(void)
{
    Window window(1280, 900);
    Context* ctx = window.getCtx();
    ctx->shaderFromFile("base", "../../assets/basic.vert.glsl", "../../assets/basic.frag.glsl");

    auto vao = ctx->vertexArrayObjectNew();
    ctx->texture2DNew("logo", "../../assets/logo.png");

    while (window.isOpen()) {
        ctx->clear();
        ctx->viewport(0, 0, 1280, 900);
        ctx->shaderUse("base");
        ctx->texture2DGet("logo")->use();
        ctx->shaderUniform("logoTexture", 0);
        vao->use();
        vao->drawArrays(DrawMode::TRIANGLES, 0, 6);
    }

    return 0;
}