cmake_minimum_required(VERSION 3.8)
project(Ay LANGUAGES CXX VERSION 0.1.0)

# Defines
set(MAIN_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/main)
set(SCENE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/scene)
set(SHADER_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/shader)
set(RENDER_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/render)

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Get files
file(GLOB_RECURSE MAIN_SOURCES ${MAIN_FOLDER}/**.cpp ${MAIN_FOLDER}/**.h)
source_group("Main" FILES ${MAIN_SOURCES})

file(GLOB_RECURSE SCENE_SOURCES ${SCENE_FOLDER}/**.cpp ${SCENE_FOLDER}/**.h)
source_group("Scene" FILES ${SCENE_SOURCES})

file(GLOB_RECURSE SHADER_SOURCES ${SHADER_FOLDER}/**.cpp ${SHADER_FOLDER}/**.h)
source_group("Shader" FILES ${SHADER_SOURCES})

file(GLOB_RECURSE RENDER_SOURCES ${RENDER_FOLDER}/**.cpp ${RENDER_FOLDER}/**.h)
source_group("Render" FILES ${RENDER_SOURCES})

add_executable(ay ${MAIN_SOURCES} ${SCENE_SOURCES} ${SHADER_SOURCES} ${RENDER_SOURCES})
target_include_directories(ay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ay ${CONAN_LIBS})
set_target_properties(ay PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    CXX_STANDARD 17)

if (MSVC)
    target_compile_options(ay PRIVATE /W4 /WX)
else ()
    target_compile_options(ay PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()