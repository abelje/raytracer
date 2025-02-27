#pragma once

#include "material.h"

class Metallic : public Material {
public:
    Metallic(Color color, bool emitting, double fuzz);

    Ray scatter(const Ray& ray, const Hit& hit) const override;

private:
    // Size of the scattering sphere, should be limited to values [0, 1]
    double fuzz;
};