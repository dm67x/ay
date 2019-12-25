cmake_minimum_required(VERSION 3.8)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

project(Ay)

option(COPY_ASSETS "Copy assets" ON)

find_package(OpenGL REQUIRED)
add_subdirectory(third_party/glm)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glad)
add_subdirectory(third_party/tinygltf)

if (COPY_ASSETS)
    add_subdirectory(assets)
endif ()

file(GLOB_RECURSE SRCFILES ${CMAKE_CURRENT_SOURCE_DIR}/code/**.cpp ${CMAKE_CURRENT_SOURCE_DIR}/code/**.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCFILES})

add_executable(ay ${SRCFILES})
target_include_directories(ay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
target_link_libraries(ay PRIVATE glm glad glfw tinygltf OpenGL::GL)