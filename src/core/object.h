#pragma once

#include <optional>
#include <utility>

class Material;
class Hit;
class Ray;

class Object {
public:
    explicit Object(const Material* material);
    virtual ~Object() = default;

    virtual std::optional<double> intersect(const Ray& ray) const = 0;
    virtual Hit construct_hit(const Ray& ray, double time) const = 0;
    virtual std::pair<double, double> uv(const Hit& hit) const = 0;

    const Material* material;
};
