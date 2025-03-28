#pragma once

#include "texture.h"

class Gradient : public Texture {
public:
    Gradient(Color a, Color b);
    Color value(double u, double v) const override;
private:
    Color a;
    Color b;
};
