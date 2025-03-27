#pragma once

#include "texture.h"
#include "color.h"

class Solid : public Texture {
public:
    Solid(Color color);
    Color value(double u, double v) const override;

private:
    Color color;
};