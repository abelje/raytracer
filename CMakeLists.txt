cmake_minimum_required(VERSION 3.30)
project(raytracer)

# adjust any compiler settings
add_compile_options(-g -Wall -Wextra -pedantic -std=c++20)

# add library for vector class and operations
add_library(vector3d vector3d.cpp)

# test program for vector3d
add_executable(test test-vector3d.cpp)
target_link_libraries(test PUBLIC vector3d)

# another program using vector3d
add_executable(main main.cpp)
target_link_libraries(main PUBLIC vector3d)

#Colors test code
add_executable(test-pixels.out
        test-pixels.cpp
        pixels.cpp
        vector3d.cpp
        lodepng.cpp
)
target_compile_options(test-pixels.out PUBLIC -Wall -Wextra -pedantic -ggdb -std=c++17)
target_link_libraries(test-pixels.out ${SOURCES})
