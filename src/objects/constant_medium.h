#pragma once

#include "object.h"

class ConstantMedium : public Object {
public:
    ConstantMedium(Object* boundary, double density, const Material* material);

    Object* boundary;
    double density;
};
