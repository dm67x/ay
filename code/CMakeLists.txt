cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE SRCFILES ${CMAKE_CURRENT_SOURCE_DIR}/**.cc ${CMAKE_CURRENT_SOURCE_DIR}/**.h)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCFILES})

add_executable(ay ${SRCFILES})
target_include_directories(ay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ay PRIVATE glm glad stb glfw OpenGL::GL)