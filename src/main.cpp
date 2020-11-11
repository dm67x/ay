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
        "uniform vec3 bg;\n"
        "void main() { fragOut = texture(logoTexture, uv) * vec4(bg, 1); }");

    PlatformId vao = ctx->vaoNew();
    ctx->textureNew("logo", "../../assets/logo.png");

    while (windowIsOpen()) {
        ctx->clear();
        ctx->shaderUse("base");
        ctx->shaderUniform("bg", Vec3(1.f, 1.f, 1.f));
        ctx->textureUse("logo");
        ctx->shaderUniform("logoTexture", 0);
        ctx->vaoBind(vao);
        ctx->drawArrays(OpenGL::DrawMode::TRIANGLES, 0, 6);
        ctx->vaoBind(0);
    }

    delete ctx;
    destroyWindow();
    return 0;
}