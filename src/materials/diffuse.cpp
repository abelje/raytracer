#include "diffuse.h"
#include "ray.h"
#include "random.h"
#include "hit.h"


Diffuse::Diffuse(const Texture* texture, bool emitting)
    :Material{texture, emitting, "Diffuse"} {}

Ray Diffuse::scatter(const Ray& ray, const Hit& hit) const {
  Vector3D scattered = random_hemisphere(hit.normal);
// Vector3D scattered = reflect(ray.direction, hit.normal);
  return Ray{hit.position, scattered};
}
