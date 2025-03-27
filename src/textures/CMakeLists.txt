add_library(textures
  solid.h solid.cpp

)
target_include_directories(textures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(textures PUBLIC core)
