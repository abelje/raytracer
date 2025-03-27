#include "metallic.h"
#include "ray.h"
#include "random.h"
#include "hit.h"

Metallic::Metallic(const Texture* texture, bool emitting, double fuzz)
    : Material(texture, emitting, "Metallic"), fuzz(fuzz) {
    if (fuzz > 1 || fuzz < 0) {
        throw std::runtime_error("Fuzz value must be in the range [0,1]");
    }
}

Ray Metallic::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D reflected = reflect(ray.direction, hit.normal);
    reflected += fuzz * random_unit_vector();
    return Ray{hit.position, reflected};
}
