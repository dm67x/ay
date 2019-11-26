cmake_minimum_required(VERSION 3.8)
project(Ay LANGUAGES CXX VERSION 0.1.0)

# Set "bin" directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Defines
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

# Third parties
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

# Libraries
set(AY_LIB "glfw;glad::glad;glm;${OPENGL_gl_LIBRARY}")

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