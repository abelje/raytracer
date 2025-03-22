#pragma once

#include <optional>

class Material;
class Hit;
class Ray;

class Object {
public:
    explicit Object(const Material* material);
    virtual ~Object() {}

    virtual std::optional<double> intersect(const Ray& ray) const = 0;
    virtual Hit construct_hit(const Ray& ray, double time) const = 0;


    const Material* material;
};
