cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(WIN32_LEAN_AND_MEAN)


project(avhook-titanfall2)

add_library(avhook MODULE source/main.cpp)