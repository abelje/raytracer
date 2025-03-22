#include "sphere.h"
#include "ray.h"
#include "constants.h"
#include <cmath>

Sphere::Sphere(const Point3D& center, double radius, const Material* material)
    :Object{material}, center{center}, radius{radius} {}

std::optional<double> Sphere::aintersect(const Ray& ray) const {
    //Insert origin ray in |P-C|^2 = r^2
    // ray.origin - center;
    //if b^2-4ac is 0, 1 hit
    // if it is positive, 2 hits
    // Vector3D oc = ray.origin - center;
    // double a = dot(ray.direction, ray.direction);
    // double b = 2 * dot(ray.direction, oc);
    // double c = dot(oc, oc) - (radius * radius);
    // double discriminant = (b*b) - (4*a*c);

    Vector3D oc = center - ray.origin;
    auto a = length(ray.direction) * length(ray.direction);
    auto h = dot(ray.direction, oc);
    auto c = length(oc) * length(oc) - radius*radius;
    auto discriminant = h*h - a*c;

    // There are only 3 options:
    // disc < -eps        => miss
    // -eps < disc < +eps => one hit
    // disc > +eps        => two hits, return closer one

    if (discriminant < -Constants::epsilon) {
        // Miss
        return std::nullopt;
    }

    // if (-Constants::epsilon < discriminant) {
        // one hit
        return (h - std::sqrt(discriminant)) / a;
    // }

    // if (discriminant > Constants::epsilon) {
    //     // two hits, return closer one
    //     double hit_1 = (-b - std::sqrt(discriminant)) / (2 * a);
    //     double hit_2 = (-b + std::sqrt(discriminant)) / (2 * a);
    //     double hit = std::min(std::abs(hit_1), std::abs(hit_2));
    //     return hit;
    // }
    // else {
    //     return std::nullopt;
    // }
}

std::optional<double> Sphere::intersect(const Ray& ray) const {
    Vector3D OC = center - ray.origin;
    double R = dot(ray.direction, OC);
    double h2 = radius*radius - dot(OC, OC) + R*R;
    if (h2 < 0) {
        return {};
    }

    double h = std::sqrt(h2);

    // time = R +/- h
    if ((R-h) > Constants::epsilon) {
        // ray intersects from outside sphere -> R-h is closer to ray origin
        // if time is positive and futher away than Epsilon
        return R-h;
    }
    else if ((R+h) > Constants::epsilon) {
        // ray intersections from inside sphere -> R+h is in the direction of the ray
        // time is positive and further away than Epsilon
        return R+h;
    }
    else {
        return {};
    }
}


Hit Sphere::construct_hit(const Ray& ray, double time) const {
    // calculate the surface normal
    Point3D point = ray.at(time);
    Vector3D normal = (point - center) / radius;
    return Hit{time, point, normal, this};
}

