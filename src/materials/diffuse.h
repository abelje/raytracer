#pragma once

#include "material.h"

class Diffuse : public Material {
public:
    Diffuse(Color color, bool emitting); // hardcode name

    Ray scatter(const Ray& ray, const Hit& hit) const override;
};