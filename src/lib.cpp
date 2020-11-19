#include "lib.hpp"
#include <cassert>

AY_LIB Window* windowNew(int width, int height) {
    return new Window(width, height);
}

AY_LIB void windowDispose(const Window* window) {
    assert(window);
    delete window;
}

AY_LIB bool windowIsOpen(Window* window) {
    assert(window);
    return window->isOpen();
}

AY_LIB Context* windowGetContext(const Window* window) {
    assert(window);
    return window->getContext();
}

AY_LIB void windowGetSize(const Window* window, int* width, int* height) {
    assert(window);
    auto size = window->getSize();
    if (width) {
        *width = size.first;
    }
    
    if (height) {
        *height = size.second;
    }
}

AY_LIB bool windowKeyPressed(const Window* window, int key) {
    assert(window);
    return window->isKeyPressed(key);
}

AY_LIB bool windowKeyReleased(const Window* window, int key) {
    assert(window);
    return window->isKeyReleased(key);
}

AY_LIB void windowGetMousePosition(const Window* window, float* x, float* y) {
    assert(window);
    auto mousePos = window->getMousePosition();
    if (x) {
        *x = mousePos.first;
    }

    if (y) {
        *y = mousePos.second;
    }
}

AY_LIB void windowClose(const Window* window) {
    assert(window);
    window->close();
}

AY_LIB Color* colorCreate(float r, float g, float b, float a) {
    return new Color(r, g, b, a);
}

AY_LIB Color* colorCreateFromHex(unsigned int hex) {
    return new Color(hex);
}

AY_LIB const Window* contextGetWindow(const Context* ctx) {
    assert(ctx);
    return &ctx->getWindow();
}

AY_LIB const char* contextGetVersion(const Context* ctx) {
    assert(ctx);
    return ctx->getVersion().c_str();
}

AY_LIB const char* contextGetVendor(const Context* ctx) {
    assert(ctx);
    return ctx->getVendor().c_str();
}

AY_LIB void contextClear(const Context* ctx, const Color* color) {
    assert(ctx);
    assert(color);
    ctx->clear(color->r, color->g, color->b, color->a);
}

AY_LIB void contextViewport(const Context* ctx, int x, int y, int w, int h) {
    assert(ctx);
    ctx->viewport(x, y, w, h);
}

AY_LIB void contextUiBegin(const Context* ctx) {
    assert(ctx);
    ctx->uiBegin();
}

AY_LIB void contextUiEnd(const Context* ctx) {
    assert(ctx);
    ctx->uiEnd();
}

AY_LIB void contextUiCreateWindow(
    const Context* ctx,
    const char* name,
    void (*draw)(),
    int flags,
    float posX, float posY,
    float sizeX, float sizeY)
{
    assert(ctx);
    assert(name);
    ctx->uiCreateWindow(std::string(name), draw, static_cast<ImGuiWindowFlags_>(flags), ImVec2(posX, posY), ImVec2(sizeX, sizeY));
}

AY_LIB void contextUiCreateText(const Context* ctx, const Color* color, const char* fmt) {
    assert(ctx);
    assert(color);
    assert(fmt);
    ctx->uiCreateText(std::string(fmt), *color);
}

AY_LIB void contextUiCreateColorEditor(const Context* ctx, Color* color, const char* fmt) {
    assert(ctx);
    assert(color);
    assert(fmt);
    ctx->uiCreateColorEditor(std::string(fmt), *color);
}

AY_LIB void contextShaderFromMemory(Context* ctx, const char* name, const char* vertex, const char* fragment) {
    assert(ctx);
    assert(name);
    assert(vertex);
    assert(fragment);
    ctx->shaderFromMemory(std::string(name), std::string(vertex), std::string(fragment));
}

AY_LIB void contextShaderFromFile(Context* ctx, const char* name, const char* vertex, const char* fragment) {
    assert(ctx);
    assert(name);
    assert(vertex);
    assert(fragment);
    ctx->shaderFromFile(std::string(name), std::string(vertex), std::string(fragment));
}

AY_LIB void contextShaderDispose(Context* ctx, const char* name) {
    assert(ctx);
    assert(name);
    ctx->shaderDispose(std::string(name));
}

AY_LIB void contextShaderUse(Context* ctx, const char* name) {
    assert(ctx);
    assert(name);
    ctx->shaderUse(std::string(name));
}

AY_LIB void contextShaderUniform1f(Context* ctx, const char* name, float value) {
    assert(ctx);
    assert(name);
    ctx->shaderUniform(std::string(name), value);
}

AY_LIB void contextShaderUniform1i(Context* ctx, const char* name, int value) {
    assert(ctx);
    assert(name);
    ctx->shaderUniform(std::string(name), value);
}

AY_LIB void contextShaderUniform3f(Context* ctx, const char* name, const glm::vec3* value) {
    assert(ctx);
    assert(name);
    assert(value);
    ctx->shaderUniform(std::string(name), *value);
}

AY_LIB void contextShaderUniform4f(Context* ctx, const char* name, const glm::vec4* value) {
    assert(ctx);
    assert(name);
    assert(value);
    ctx->shaderUniform(std::string(name), *value);
}

AY_LIB void contextShaderUniformMatrix(Context* ctx, const char* name, const glm::mat4* value) {
    assert(ctx);
    assert(name);
    assert(value);
    ctx->shaderUniform(std::string(name), *value);
}