#pragma once

struct GLFWwindow;
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
    Context* getContext() const;
};