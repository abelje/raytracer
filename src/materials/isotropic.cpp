#include "isotropic.h"
#include "random.h"
#include "ray.h"
#include "hit.h"

Isotropic::Isotropic(const Texture* texture, bool emitting)
    :Material{texture, emitting} {}

Ray Isotropic::scatter(const Ray& ray, const Hit& hit) const {
    Ray scattered{hit.position, random_unit_vector()};
    return scattered;
}
