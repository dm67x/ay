cmake_minimum_required(VERSION 3.8)
project(Ay LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(glm REQUIRED)
find_package(glad REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/code/**.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES})

add_executable(ay ${SRC_FILES})
target_include_directories(ay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
target_link_libraries(ay PRIVATE SDL2::SDL2 SDL2::SDL2main glm glad::glad OpenGL::GL assimp::assimp)
set_target_properties(ay PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    CXX_STANDARD 17)

if (MSVC)
    target_compile_options(ay PRIVATE /W3 /WX)
else ()
    target_compile_options(ay PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()

# Copy shaders
file(GLOB SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl)
file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/shaders)