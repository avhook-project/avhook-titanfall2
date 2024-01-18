project(imgui)
add_library(imgui STATIC src/imgui.cpp)
target_include_directories(imgui PUBLIC include)

add_subdirectory(src)
add_subdirectory(extlibs)

target_link_libraries(imgui PRIVATE freetype)
