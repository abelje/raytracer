add_library(materials
lambertian.h lambertian.cpp)

target_link_libraries(materials PUBLIC core)
target_include_directories(materials PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})