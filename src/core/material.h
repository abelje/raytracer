#pragma once

#include "color.h"

class Hit;
class Ray;
class Texture;

class Material {
public:
    Material(const Texture* texture , bool emitting, std::string name = " ");

    virtual Ray scatter(const Ray& ray, const Hit& hit) const = 0;
    virtual ~Material() = default;

    std::string name;
    const Texture* texture;
    bool emitting;
};

std::ostream& operator<<(std::ostream& os, const Material& material);

Vector3D reflect(const Vector3D& v, const Vector3D& normal);
Vector3D refract(const Vector3D& v, const Vector3D& normal, double index_ratio);
double schlick(double cos_theta, double index_ratio);