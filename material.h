#pragma once

#include "color.h"

class Hit;
class Ray;

class Material {
public:
    Material(std::string name, Color color, bool emitting);

    virtual Ray scatter(const Ray& ray, const Hit& hit) const = 0;

private:
    std::string name;
    Color color;
    bool emitting;
};

std::ostream& operator<<(std::ostream& os, const Material& material);