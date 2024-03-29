cmake_minimum_required(VERSION 3.12)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

project(ay 
    LANGUAGES CXX C
    HOMEPAGE_URL "https://github.com/dm67x/ay"
    DESCRIPTION "3D Render Engine"
    VERSION "0.3")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/stb)
add_subdirectory(extern/spdlog)
add_subdirectory(extern/glm)

include_directories(extern/glfw/include)
include_directories(extern/glad)
include_directories(extern/stb)

add_executable(ay  
    "src/main.cpp"
    "src/camera.cpp"
    "src/model.cpp"
    "src/model_node.cpp"
    "src/window.cpp" 
    "src/context.cpp"
    "src/tiny_gltf.cpp"
    "src/imgui/imgui_impl_glfw.cpp"
    "src/imgui/imgui_impl_opengl3.cpp"
    "src/imgui/imgui_widgets.cpp"
    "src/imgui/imgui.cpp"
    "src/imgui/imgui_draw.cpp")
target_link_libraries(ay PRIVATE glad glfw glm stb spdlog OpenGL::GL)
target_compile_options(ay PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)