#include "world.h"

#include "constants.h"
#include "object.h"
#include "sphere.h"

void World::add(Point3D center, double radius, const Material* material) {
  objects.push_back(std::make_unique<Sphere>(center, radius, material));
}

std::optional<Hit> World::find_nearest(const Ray& ray) const {
	double time = Constants::infinity;
    Sphere* nearest = nullptr;
    for (const auto& object : objects) {
      std::optional<double> t = object->intersect(ray);
      if(t.has_value() && t.value() < time) {
      	nearest = object.get();
        time = t.value();
      }
    }
    if(nearest) {
      Hit hit = nearest->construct_hit(ray, time);
      return hit;
    }
    else {
      return {};
    }
}
