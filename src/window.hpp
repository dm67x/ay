#pragma once

struct GLFWwindow;

///
/// @brief Create a new window
/// @param width Width
/// @param height Height
///
void createWindow(int width, int height);

///
/// @brief Destroy the window
///
void destroyWindow();

///
/// @brief Get current window
/// @return The current window
///
GLFWwindow* getWindow();

///
/// @brief Get the window state
/// @return True if window is open false otherwise
///
bool windowIsOpen();