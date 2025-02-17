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

# png support
add_library(png lodepng.h lodepng.cpp)

add_library(pixels pixels.h pixels.cpp color.h)
target_link_libraries(pixels PRIVATE png PUBLIC vector3d)

#testing pixels class
add_executable(test-pixels test-pixels.cpp)
target_link_libraries(test-pixels PRIVATE pixels)

# shapes
add_library(shapes
    ray.h ray.cpp
    sphere.h sphere.cpp
    hit.h hit.cpp
    timer.h timer.cpp)
target_link_libraries(shapes PUBLIC vector3d)

add_executable(test-intersect test-intersect.cpp)
target_link_libraries(test-intersect PRIVATE shapes PRIVATE png PUBLIC pixels)

add_library(camera
    camera.h camera.cpp)

add_executable(test-camera test-camera.cpp)
target_link_libraries(test-camera PRIVATE camera PRIVATE shapes PRIVATE png PUBLIC pixels)

add_library(world
    world.h world.cpp)

add_executable(test-world test-world.cpp)
target_link_libraries(test-world PRIVATE camera PRIVATE world PRIVATE shapes PRIVATE png PUBLIC pixels)
