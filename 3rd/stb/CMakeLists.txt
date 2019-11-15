cmake_minimum_required(VERSION 3.8)
project(stb)
add_library(stb STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/libstb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h
)