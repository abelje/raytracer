#include "constant_medium.h"

#include "constants.h"
#include "hit.h"
#include "ray.h"

ConstantMedium::ConstantMedium(Object *boundary, double density, const Material* material)
    :Object{material}, boundary{boundary}, density{density} {}

std::optional<double> ConstantMedium::intersect(const Ray& ray) const {
    auto t1 = boundary->intersect(ray);
    if (!t1) {
        return {};
    }

    Hit hit = boundary->construct_hit(ray, t1.value);
    Ray ray1{ray.at(t1.value() + 2*Constants::epsilon), ray.direction};
    auto t2 = boundary->intersect(ray1);
}

