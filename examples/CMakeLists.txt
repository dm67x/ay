cmake_minimum_required(VERSION 3.8)

find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(01_example ${CMAKE_CURRENT_SOURCE_DIR}/01/main.cpp)
target_include_directories(01_example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/01)
target_link_libraries(01_example PRIVATE ay SDL2::SDL2 SDL2::SDL2main glm glad::glad OpenGL::GL)
set_target_properties(01_example PROPERTIES CXX_STANDARD 17)

if (MSVC)
    target_compile_options(01_example PRIVATE /W4 /WX)
else ()
    target_compile_options(01_example PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()