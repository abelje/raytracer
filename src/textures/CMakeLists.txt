add_library(textures
  solid.h solid.cpp
  gradient.h gradient.cpp
  image.h image.cpp
  surface-normal.h surface-normal.cpp
  checkerboard.h checkerboard.cpp
  spiral.h spiral.cpp
)
target_include_directories(textures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(textures PUBLIC core image)
