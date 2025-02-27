#include "metallic.h"
#include "ray.h"
#include "random.h"
#include "hit.h"

Metallic::Metallic(Color color, bool emitting, double fuzz)
    : Material("Metallic", color, emitting), fuzz(fuzz) {}

Ray Metallic::scatter(const Ray& ray, const Hit& hit) const {

}
