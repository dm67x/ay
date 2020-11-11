#include "window.hpp"
#include "context.hpp"
#include "math.hpp"

int main(void)
{
    createWindow(1280, 900);
    Context* ctx = new Context();
    ctx->shaderFromMemory("base", 
        "#version 320 es\n"
        "layout(location = 0) out vec2 uv;\n"
        "void main() {\n" 
        "float x = float(((uint(gl_VertexID) + 2u) / 3u)%2u); float y = float(((uint(gl_VertexID) + 1u) / 3u)%2u);\n"
        "gl_Position = vec4(-1.0f + x*2.0f, -1.0f+y*2.0f, 0.0f, 1.0f); uv = vec2(x, y); }", 
        "#version 320 es\n"
        "precision mediump float;\n"
        "in vec2 uv;\n"
        "out vec4 fragOut;\n"
        "uniform sampler2D logoTexture;\n"
        "void main() { fragOut = texture(logoTexture, uv) * vec4(1); }");

    PlatformId vao = ctx->vaoNew();
    ctx->textureNew("logo", "../../assets/logo.png");

    while (windowIsOpen()) {
        ctx->clear();
        ctx->viewport(0, 0, windowGetWidth(), windowGetHeight());
        ctx->shaderUse("base");
        ctx->textureUse("logo");
        ctx->shaderUniform("logoTexture", 0);
        ctx->vaoUse(vao);
        ctx->drawArrays(Platform::DrawMode::TRIANGLES, 0, 6);
        ctx->vaoUse(0);
    }

    delete ctx;
    destroyWindow();
    return 0;
}