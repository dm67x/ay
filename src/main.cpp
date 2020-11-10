#include "window.hpp"
#include "context.hpp"

int main(void)
{
    createWindow(1280, 900);
    Context* ctx = new Context();
    ctx->newShaderFromMemory("base", 
        "#version 320 es\n"
        "void main() { gl_Position = vec4(1); }", 
        "#version 320 es\n"
        "precision mediump float;\n"
        "out vec4 fragOut;\n"
        "void main() { fragOut = vec4(1); }");

    while (windowIsOpen()) {
        ctx->useShader("base");
    }

    delete ctx;
    destroyWindow();
    return 0;
}