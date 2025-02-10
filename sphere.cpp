#include "sphere.h"

#include <complex>
#include <math.h>

#include "constants.h"

Sphere::Sphere(const Point3D& center, double radius)
    :center{center}, radius{radius} {}

std::optional<double> Sphere::aintersect(const Ray& ray) const {
    //Insert origin ray in |P-C|^2 = r^2
    // ray.origin - center;
    //if b^2-4ac is 0, 1 hit
    // if it is positive, 2 hits
    double a = dot(ray.direction, ray.direction);
    double b = 2 * dot(ray.direction, ray.origin - center);
    double c = dot(ray.origin - center, ray.origin - center) - (radius * radius);
    double discriminant = (b*b) - (4*a*c);
    if (discriminant < 0 + Constants::epsilon && discriminant > 0 - Constants::epsilon) {
        // std::cout << "Hit ";
        // (-b +- discriminant) / 2a
        return -b + discriminant / (2 * a);
    }
    else if (discriminant > 0 - Constants::epsilon) {
        // std::cout << "Hit ";
        return -b - discriminant / (2 * a);
    }
    else if (discriminant == 0) {
        // std::cout << "Hit ";
        return -b + (discriminant / (2 * a));
    }
    else if (discriminant > 0) { //ray intersects the sphere
        // std::cout << "Hit ";
        return -b + (discriminant / (2 * a));
    }
    else {
        // std::cout << "Miss! ";
        return std::nullopt;
    }
}

std::optional<double> Sphere::intersect(const Ray& ray) const {
    // if q^2 > r^2 miss
    auto temp = center - ray.origin;
    double temp2 = dot(temp, temp);
    double q2 = -dot(ray.origin, temp) + temp2;
    if (q2 > radius * radius) {
        // std::cout << "miss!\n";
        return std::nullopt;
    }
    if (q2 <= radius * radius || q2 + Constants::epsilon < radius * radius
        || q2 - Constants::epsilon > radius * radius) { // hit
        // v dot (c - p) - h
        // h = sqrt(r^2-q^2)
        return dot(ray.origin, temp) - sqrt(radius * radius - q2);
    }
    else {
        return std::nullopt;
    }
}


Hit Sphere::construct_hit(const Ray& ray, double time) const {
    // calculate the surface normal
    Point3D point = ray.at(time);
    Vector3D normal = (point - center) / radius;
    return Hit{time, point, normal};
}

