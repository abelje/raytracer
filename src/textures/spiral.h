#pragma once

#include "texture.h"
#include "color.h"

class Spiral : public Texture {
public:
    Spiral(Color a, Color b, int num_spirals, bool flip = false);
    Color value(double u, double v) const override;
private:
    Color a;
    Color b;
    int num_spirals;
    bool flip;
};
