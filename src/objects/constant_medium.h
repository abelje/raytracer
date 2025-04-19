#pragma once

#include "object.h"

class ConstantMedium : public Object {
public:
    ConstantMedium(Object* boundary, double density, const Material* material);

    virtual AABB bounding_box() const = 0;
    virtual std::optional<double> intersect(const Ray& ray) const override;
    virtual Hit construct_hit(const Ray& ray, double time) const override;
    virtual std::pair<double, double> uv(const Hit& hit) const override;

    Object* boundary;
    double density;
};
