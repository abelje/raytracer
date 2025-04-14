#include "constant_medium.h"

ConstantMedium::ConstantMedium(Object *boundary, double density, const Material* material)
    :Object{material}, boundary{boundary}, density{density} {}


