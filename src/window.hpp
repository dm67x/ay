#pragma once

#include <utility>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Context;

class Window {
    GLFWwindow* window;
    Context* ctx;

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
    bool isOpen() const;

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
};