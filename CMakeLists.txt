cmake_minimum_required(VERSION 3.12)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(ay 
    LANGUAGES CXX C
    HOMEPAGE_URL "https://github.com/dm67x/ay"
    DESCRIPTION "3D Render Engine"
    VERSION "0.2")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/stb)

include_directories(extern/glad)
include_directories(extern/stb)

add_executable(ay "src/main.cpp" "src/window.cpp" "src/mesh.cpp" "src/context.cpp")
target_link_libraries(ay PRIVATE glad glfw OpenGL::GL)
target_compile_options(ay PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)