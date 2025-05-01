#include "rectangle.h"

#include <cmath>

#include "hit.h"
#include "aabb.h"
#include "ray.h"

Rectangle::Rectangle(const Point3D& bottom_left, const Point3D& bottom_right, double width, const Material* material)
    :Object{material}, top{bottom_left,{bottom_left.x, bottom_left.y, bottom_left.z + width},
        bottom_right, material},
    bottom{{bottom_right.x, bottom_right.y, bottom_right.z + width},{bottom_left.x, bottom_left.y,
        bottom_left.z + width},bottom_right, material} {}

Rectangle::Rectangle(const Point3D& bottom_left, const Point3D& bottom_right, const Point3D& top_left, const Point3D& top_right, const Material *material)
    :Object{material}, top{bottom_left, bottom_right, top_right, material}, bottom{top_left, top_right, bottom_left, material} {}

AABB Rectangle::bounding_box() const {
    double xmin = std::min(top.vertex0.x, std::min(top.vertex1.x, top.vertex2.x));
    double xmax = std::max(top.vertex0.x, std::max(top.vertex1.x, top.vertex2.x));
    double ymin = std::min(bottom.vertex0.y, std::min(bottom.vertex1.y, bottom.vertex2.y));
    double ymax = std::max(top.vertex0.y, std::max(top.vertex1.y, top.vertex2.y));
    double zmin = std::min(bottom.vertex0.z, std::min(bottom.vertex1.z, bottom.vertex2.z));
    double zmax = std::max(top.vertex0.z, std::max(top.vertex1.z, top.vertex2.z));

    Point3D min{xmin, ymin, zmin}, max{xmax, ymax, zmax};
    return AABB{min, max};
}

std::optional<double> Rectangle::intersect(const Ray& ray) const {
    auto top_hit = top.intersect(ray);
    auto bottom_hit = bottom.intersect(ray);

    if (top_hit && bottom_hit) {
        // return which ever hit is closer
        if (*top_hit < *bottom_hit) {
            return top_hit;
        }
        else {
            return bottom_hit;
        }
    }
    if (top_hit) {
        return top_hit;
    }
    else {
        return bottom_hit;
    }
}

Hit Rectangle::construct_hit(const Ray& ray, double time) const {
    Point3D point = ray.at(time);
    // ensure that the ray and surface normals are pointing in opposite directions
    bool top_negative = std::signbit(dot(ray.direction, top.normal));
    bool bot_negative = std::signbit(dot(ray.direction, bottom.normal));

    if (top_negative && bot_negative) {
        return Hit{time, point, top.normal, this};
    }
    else {
        return Hit{time, point, -top.normal, this};
    }
}

std::pair<double, double> Rectangle::uv(const Hit& hit) const {
    Vector3D P = hit.position - top.vertex0;
    double u = dot(P, top.edge2) / dot(top.edge2, top.edge2); // percentage of the side ratio
    double v = dot(P, top.edge1) / dot(top.edge1, top.edge1);
    return {u, 1.0 - v};
}
