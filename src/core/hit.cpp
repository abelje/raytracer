#include "hit.h"
#include "object.h"

Hit::Hit(double time, const Point3D& position, const Vector3D& normal, const Object* object)
    :time{time}, position{position}, normal{unit(normal)}, object{object} {}