#pragma once

#include <utility>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Context;

class Window {
    GLFWwindow* window;
    Context* ctx;
    bool cursorVisible;

    friend class Context;

public:
    /// 
    /// @brief Constructor
    /// @param width Window width
    /// @param height Window height
    /// 
    explicit Window(int width, int height);
    
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
    inline std::pair<int, int> getSize() const {
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        return std::make_pair(w, h);
    }

    ///
    /// @brief isKeyPressed
    /// @param key Keycode
    /// @return True if key pressed false otherwise
    ///
    inline bool isKeyPressed(int key) const {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    ///
    /// @brief isKeyReleased
    /// @param key Keycode
    /// @return True if key released false otherwise
    ///
    inline bool isKeyReleased(int key) const {
        return glfwGetKey(window, key) == GLFW_RELEASE;
    }

    ///
    /// @brief getMousePosition
    /// @return Mouse position
    ///
    inline std::pair<float, float> getMousePosition() const {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return std::make_pair(static_cast<float>(x), static_cast<float>(y));
    }

    ///
    /// @brief Close the window
    ///
    inline void close() const {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
};