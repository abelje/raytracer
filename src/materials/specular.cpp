#include "specular.h"
#include "ray.h"
#include "random.h"
#include "hit.h"

Specular::Specular(Color color, bool emitting)
    : Material("Specular",color, emitting) {}

Ray Specular::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D scattered = reflect(ray.direction, hit.normal);
    return Ray{hit.position, scattered};
}

