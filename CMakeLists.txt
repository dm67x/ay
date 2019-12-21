cmake_minimum_required(VERSION 3.8)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

project(Ay)

find_package(OpenGL REQUIRED)
add_subdirectory(third_party/glm)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/stb)
add_subdirectory(third_party/glad)
add_subdirectory(code)