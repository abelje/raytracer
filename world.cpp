#include "world.h"
#include "sphere.h"

void World::add(Point3D center, double radius) {
  objects.push_back(std::make_unique<Sphere>(center, radius));
}

std::optional<Hit> World::find_nearest(const Ray& ray) const {

}