#pragma once
#include <optional>

#include "point3d.h"
#include "hit.h"

class Ray;
class Material;

class Sphere {
public:
    Sphere(const Point3D& center, double radius, const Material* material = nullptr);

    // returns the distance if the ray intersects this sphere, otherwise std::nullopt
    std::optional<double> aintersect(const Ray& ray) const;
    std::optional<double> intersect(const Ray& ray) const;
    Hit construct_hit(const Ray& ray, double time) const;

    Point3D center;
    double radius;

    const Material* material;
};
