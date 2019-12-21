cmake_minimum_required(VERSION 3.8)
add_library(stb STATIC libstb.c)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})