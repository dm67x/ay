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
}