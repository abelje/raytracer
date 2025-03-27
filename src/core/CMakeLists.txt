add_library(core
        color.h
        constants.h
        hit.h hit.cpp
        material.h material.cpp
        object.h object.cpp
        point3d.h
        random.h random.cpp
        xorshiro256.h xorshiro256.cpp
        xorshiftrplus.h xorshiftrplus.cpp
        ray.h ray.cpp
        vector3d.h vector3d.cpp
        timer.h timer.cpp
        texture.h texture.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})