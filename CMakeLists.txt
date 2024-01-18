cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}")

project(avhook-titanfall2)
add_library(avhook-titanfall2 MODULE source/main.cpp)

add_subdirectory(extlibs)
add_subdirectory(source)

target_compile_definitions(avhook-titanfall2 PRIVATE WIN32_LEAN_AND_MEAN)
target_include_directories(avhook-titanfall2 PRIVATE source)
target_link_libraries(avhook-titanfall2 PRIVATE d3d11)
target_link_libraries(avhook-titanfall2 PRIVATE minhook)
target_link_libraries(avhook-titanfall2 PRIVATE titanfall2-sdk)
target_link_libraries(avhook-titanfall2 PRIVATE imgui)