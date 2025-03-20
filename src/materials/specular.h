#pragma once

#include "material.h"

class Specular : public Material {
public:
    Specular(Color color, bool emitting);

    Ray scatter(const Ray& ray, const Hit& hit) const override;
};
