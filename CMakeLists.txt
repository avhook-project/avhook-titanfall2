cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}")


project(avhook-titanfall2)
add_library(avhook-titanfall2 MODULE source/main.cpp)

target_compile_definitions(avhook-titanfall2 PRIVATE WIN32_LEAN_AND_MEAN)