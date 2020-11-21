#pragma once

#include "types.hpp"
#include <utility>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Context;

class Window {
public:
    /// 
    /// @brief Constructor
    /// @param width Window width
    /// @param height Window height
    /// 
    explicit Window(i32 width, i32 height);
    
    /// 
    /// @brief Destructor
    /// 
    ~Window();

    /// 
    /// @brief IsOpen
    /// @return True if window is open false otherwise
    /// 
    bool isOpen();

    ///
    /// @brief Get context
    /// @return Context
    ///
    inline Context* getContext() const {
        return ctx;
    }

    ///
    /// @brief Get window size
    /// @return (Width, Height)
    ///
    inline std::pair<i32, i32> getSize() const {
        i32 w, h;
        glfwGetWindowSize(window, &w, &h);
        return std::make_pair(w, h);
    }

    ///
    /// @brief isKeyPressed
    /// @param key Keycode
    /// @return True if key pressed false otherwise
    ///
    inline bool isKeyPressed(i32 key) const {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    ///
    /// @brief isKeyReleased
    /// @param key Keycode
    /// @return True if key released false otherwise
    ///
    inline bool isKeyReleased(i32 key) const {
        return glfwGetKey(window, key) == GLFW_RELEASE;
    }

    ///
    /// @brief getMousePosition
    /// @return Mouse position
    ///
    inline std::pair<f32, f32> getMousePosition() const {
        f64 x, y;
        glfwGetCursorPos(window, &x, &y);
        return std::make_pair((f32)x, (f32)y);
    }

    ///
    /// @brief Close the window
    ///
    inline void close() const {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

private:
    friend class Context;

private:
    GLFWwindow* window;
    Context* ctx;
    bool cursorVisible;
    bool cursorProcessed;
};