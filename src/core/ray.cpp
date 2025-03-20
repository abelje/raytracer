#include "ray.h"

Ray::Ray(const Point3D& origin, const Vector3D& direction)
    :origin{origin}, direction{unit(direction)} {}

Point3D Ray::at(const double time) const {
  return origin + direction * time;
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
  return os << "Ray(origin=" << ray.origin << ", direction=" << ray.direction << ")";
}