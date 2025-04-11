#include "rectangle.h"

#include <cmath>

#include "hit.h"
#include "aabb.h"
#include "ray.h"

Rectangle::Rectangle(const Point3D& top_left, double length, double width, const Material* material)
    :Object{material}, top{top_left,{top_left.x, top_left.y, top_left.z + width},
        {top_left.x + length, top_left.y, top_left.z}, material},
    bottom{{top_left.x + length, top_left.y, top_left.z + width},{top_left.x, top_left.y,
        top_left.z + width},{top_left.x + length, top_left.y, top_left.z}, material} {}

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
    double u = dot(P, top.edge2)/ length(top.edge2);
    double v = dot(P, top.edge1)/ length(top.edge1);

    double aspect_ratio = length(top.edge1) / length(top.edge2);  // Rectangle aspect ratio (width / height)
    u *= aspect_ratio;  // Scale u coordinate to match aspect ratio

    return {u, 1.0 - v};
}
