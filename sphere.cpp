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
    // if q^2 > r^2 miss
    auto temp = center - ray.origin;
    double temp2 = dot(temp, temp);
    double q2 = -dot(ray.origin, temp) + temp2;
    if (q2 > radius * radius) {
        // std::cout << "miss!\n";
        return std::nullopt;
    }

    if (q2 < -Constants::epsilon) {
        // miss
        return std::nullopt;
    }

    if (q2 - Constants::epsilon <= radius * radius < q2 + Constants::epsilon) {
        // v dot (c - p) - h
        // h = sqrt(r^2-q^2)
        return dot(ray.origin, temp) - sqrt(radius * radius - q2);
    }
    if (q2 <= radius * radius + Constants::epsilon) {
        //two hits, return closer one
        double hit_1 = dot(ray.origin, temp) - sqrt(radius * radius - q2);
        double hit_2 = dot(ray.origin, temp) + sqrt(radius * radius - q2);
        double hit = std::min(std::abs(hit_1), std::abs(hit_2));
        return hit;
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

