#include "specular.h"
#include "ray.h"
#include "random.h"
#include "hit.h"

Specular::Specular(const Texture* texture, bool emitting)
    : Material(texture, emitting, "Specular") {}

Ray Specular::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D scattered = reflect(ray.direction, hit.normal);
    return Ray{hit.position, scattered};
}

