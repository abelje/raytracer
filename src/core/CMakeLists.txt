add_library(core
        color.h
        constants.h
        hit.h hit.cpp
        material.h material.cpp
        #object.h object.cpp
        point3d.h
        random.h random.cpp
        ray.h ray.cpp
        vector3d.h vector3d.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})