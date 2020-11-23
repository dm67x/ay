cmake_minimum_required(VERSION 3.12)

add_library(imgui STATIC "imgui.cpp" "imgui_draw.cpp" "imgui_impl_glfw.cpp" "imgui_impl_opengl3.cpp" "imgui_widgets.cpp")
target_include_directories(imgui PUBLIC ../../include/imgui)