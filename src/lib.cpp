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

AY_LIB void contextClear(const Context* ctx, float r, float g, float b, float a) {
    assert(ctx);
    ctx->clear(r, g, b, a);
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