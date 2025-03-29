#include "triangle.h"
#include "constants.h"
#include "ray.h"
#include "hit.h"

Triangle::Triangle(const Point3D& vertex0, const Point3D& vertex1, const Point3D& vertex2, const Material *material)
    :Object{material}, vertex0 {vertex0}, vertex1{vertex1}, vertex2{vertex2}, edge1{vertex1-vertex0},
    edge2{vertex2-vertex0}, normal{unit(cross(edge1, edge2))} {}


std::optional<double> Triangle::intersect(const Ray& ray) const {
    // Moeller-Trumbore triangle intersection
    Vector3D h = cross(ray.direction, edge2);
    double a = dot(edge1, h);
    if (a > -Constants::epsilon && a < Constants::epsilon) {
        // ray is parallel to the triangle
        return {};
    }

    double f = 1.0 / a;
    Vector3D s = ray.origin - vertex0;
    double u = f * dot(s,h);
    if (u < 0.0 || u > 1.0) {
        // ray is outside of u coordinates of [0, 1]
        return {};
    }

    Vector3D q = cross(s, edge1);
    double v = f * dot(edge2, q);
    if (v < 0.0 || v > 1.0) {
        return {};
    }

    double t = f * dot(edge2, q);
    if (t > Constants::epsilon) {
        return t;
    }
    else {
        return {};
    }
}

Hit Triangle::construct_hit(const Ray& ray, double time) const {

}
std::pair<double,double> Triangle::uv(const Hit& hit) const {

}