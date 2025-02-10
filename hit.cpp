#include "hit.h"

Hit::Hit(double time, const Point3D& position, const Vector3D& normal)
    :time{time}, position{position}, normal{normal} {}