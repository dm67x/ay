cmake_minimum_required(VERSION 3.8)
project(Ay LANGUAGES CXX VERSION 0.1.0)

# Defines
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Get files
file(GLOB_RECURSE SRC_FILES ${SRC_PATH}/**.cpp ${SRC_PATH}/**.h)
source_group(TREE ${SRC_PATH} FILES ${SRC_FILES})

add_executable(ay ${SRC_FILES})
target_include_directories(ay PUBLIC ${SRC_PATH})
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