#pragma once

#include "triangle.h"
#include "object.h"

class Rectangle : public Object {
public:
    Rectangle(const Point3D& bottom_left, const Point3D& bottom_right, double width, const Material* material);

    AABB bounding_box() const override;
    std::optional<double> intersect(const Ray& ray) const override;
    Hit construct_hit(const Ray& ray, double time) const override;
    std::pair<double, double> uv(const Hit &hit) const override;

private:
    Triangle top, bottom;
};
