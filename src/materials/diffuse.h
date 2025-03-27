#pragma once

#include "material.h"

class Diffuse : public Material {
public:
    Diffuse(const Texture* texture, bool emitting); // hardcode name

    Ray scatter(const Ray& ray, const Hit& hit) const override;
};