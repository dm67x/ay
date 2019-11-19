cmake_minimum_required(VERSION 3.8)
project(Ay LANGUAGES CXX VERSION 0.1.0)

# Set "bin" directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Defines paths
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(3RD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# Remove GLFW documentation, tests and examples build
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Remove GLM tests
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

# Third parties
add_subdirectory(${3RD_PATH}/glfw)
add_subdirectory(${3RD_PATH}/glad)
add_subdirectory(${3RD_PATH}/stb)
add_subdirectory(${3RD_PATH}/glm)

include_directories(
    ${3RD_PATH}/glfw/include
    ${3RD_PATH}/glad/include
    ${3RD_PATH}/stb
    ${3RD_PATH}/glm)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Libraries
set(AY_LIB "glfw;glad;glm_static;stb;${OPENGL_gl_LIBRARY}")

if (MINGW)
    list(APPEND AY_LIB mingw32)
endif ()

# Get files
file(GLOB_RECURSE SRC_FILES ${SRC_PATH}/**.cpp ${SRC_PATH}/**.h)
source_group(TREE ${SRC_PATH} FILES ${SRC_FILES})

add_executable(ay ${SRC_FILES})
target_include_directories(ay PUBLIC ${SRC_PATH})
target_link_libraries(ay ${AY_LIB})
set_target_properties(ay PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    CXX_STANDARD 17)

if (MSVC)
    target_compile_options(ay PRIVATE /W4 /WX)
else ()
    target_compile_options(ay PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()