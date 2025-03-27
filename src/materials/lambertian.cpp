#include "lambertian.h"
#include "ray.h"
#include "random.h"
#include "hit.h"

Lambertian::Lambertian(const Texture* texture, bool emitting)
    : Material(texture, emitting, "Lambertian") {}

Ray Lambertian::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D scattered = hit.normal + random_unit_vector();
    return Ray{hit.position, scattered};
}