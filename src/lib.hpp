#pragma once

#ifdef _WIN32
#ifdef AY_EXPORTS
#define AY_LIB __declspec(dllexport)
#else
#define AY_LIB __declspec(dllimport)
#endif
#else
#define AY_LIB
#endif

#include "window.hpp"
#include "context.hpp"
#include "color.hpp"

extern "C" {
    /// 
    /// @brief Create a new render window
    /// @param width Width
    /// @param height Height
    /// @return Window instance
    /// 
    AY_LIB Window* windowNew(int width, int height);

    /// 
    /// @brief Destroy the render window
    /// @param window Window instance
    /// 
    AY_LIB void windowDispose(const Window* window);

    /// 
    /// @brief Check if window is open
    /// @param window Window instance
    /// @return True if window is open false otherwise
    /// 
    AY_LIB bool windowIsOpen(Window* window);

    /// 
    /// @brief Get window context
    /// @param window Window instance
    /// @return Context instance
    /// 
    AY_LIB Context* windowGetContext(const Window* window);

    /// 
    /// @brief Get window size
    /// @param window Window instance
    /// @param width Width
    /// @param height Height
    /// 
    AY_LIB void windowGetSize(const Window* window, int* width, int* height);

    /// 
    /// @brief isKeyPressed
    /// @param window Window instance
    /// @param key Keycode
    /// @param True if key pressed false otherwise
    /// 
    AY_LIB bool windowKeyPressed(const Window* window, int key);

    /// 
    /// @brief isKeyReleased
    /// @param window Window instance
    /// @param key Keycode
    /// @param True if key released false otherwise
    /// 
    AY_LIB bool windowKeyReleased(const Window* window, int key);

    /// 
    /// @brief getMousePosition
    /// @param window Window instance
    /// @param x Mouse X
    /// @param y Mouse Y
    /// 
    AY_LIB void windowGetMousePosition(const Window* window, float* x, float* y);

    /// 
    /// @brief Close the window
    /// @param window Window instance
    /// 
    AY_LIB void windowClose(const Window* window);

    /// 
    /// @brief Create a new color from rgba
    /// @param r Red
    /// @param g Green
    /// @param b Blue
    /// @param a Alpha
    /// @return Color instance
    /// 
    AY_LIB Color* colorCreate(float r, float g, float b, float a);

    /// 
    /// @brief Create a new color from hex
    /// @param hex Hexadecimal value
    /// @return Color instance
    /// 
    AY_LIB Color* colorCreateFromHex(unsigned int hex);

    /// 
    /// @brief Get window attached to context
    /// @param ctx Context instance
    /// @return Window instance
    /// 
    AY_LIB const Window* contextGetWindow(const Context* ctx);

    /// 
    /// @brief Get OpenGL version
    /// @param ctx Context instance
    /// @return OpenGL version string
    /// 
    AY_LIB const char* contextGetVersion(const Context* ctx);

    /// 
    /// @brief Get Vendor
    /// @param ctx Context instance
    /// @return Vendor string
    /// 
    AY_LIB const char* contextGetVendor(const Context* ctx);

    /// 
    /// @brief Clear screen
    /// @param ctx Context instance
    /// @param color Color
    /// 
    AY_LIB void contextClear(const Context* ctx, const Color* color);

    /// 
    /// @brief Set viewport
    /// @param ctx Context instance
    /// @param x Left
    /// @param y Top
    /// @param w Width
    /// @param h Height
    /// 
    AY_LIB void contextViewport(const Context* ctx, int x, int y, int w, int h);

    /// 
    /// @brief Begin ImGUI UI
    /// @param ctx Context instance
    /// 
    AY_LIB void contextUiBegin(const Context* ctx);

    /// 
    /// @brief Begin ImGUI UI
    /// @param ctx Context instance
    /// 
    AY_LIB void contextUiBegin(const Context* ctx);

    /// 
    /// @brief Render ImGUI UI
    /// @param ctx Context instance
    /// 
    AY_LIB void contextUiEnd(const Context* ctx);

    ///
    /// @brief Create a new ImGUI Window
    /// @param ctx Context instance
    /// @param name Window name
    /// @param flags Window Flags
    /// @param draw Draw
    /// @param posX Position X
    /// @param posY Position Y
    /// @param sizeX Size X
    /// @param sizeY Size Y
    /// 
    AY_LIB void contextUiCreateWindow(
        const Context* ctx, 
        const char* name, 
        void (*draw)(), 
        int flags, 
        float posX, float posY, 
        float sizeX, float sizeY);

    ///
    /// @brief Create a new UI text
    /// @param ctx Context instance
    /// @param color Color instance
    /// @param fmt Text
    ///
    AY_LIB void contextUiCreateText(const Context* ctx, const Color* color, const char* fmt);

    ///
    /// @brief Create a new UI color editor
    /// @param ctx Context instance
    /// @param color Color instance
    /// @param fmt Text
    ///
    AY_LIB void contextUiCreateColorEditor(const Context* ctx, Color* color, const char* fmt);
}