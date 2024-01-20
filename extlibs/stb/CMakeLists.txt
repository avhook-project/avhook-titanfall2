project(stb-image)
add_library(stb-image STATIC stb_image.cpp)
target_include_directories(stb-image PUBLIC include)
