cmake_minimum_required(VERSION 3.8)
project(ay LANGUAGES CXX VERSION 0.1.0)

find_package(glm REQUIRED)
find_package(glad REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE 
    SRC_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/**.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES})

add_library(ay SHARED ${SRC_FILES})
target_include_directories(ay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_link_libraries(ay PRIVATE SDL2::SDL2 SDL2::SDL2main glm glad::glad OpenGL::GL)
target_compile_definitions(ay PRIVATE ay_EXPORTS)
set_target_properties(ay PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    CXX_STANDARD 17)

if (MSVC)
    target_compile_options(ay PRIVATE /W3 /WX)
else ()
    target_compile_options(ay PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()