#pragma once

#include "texture.h"
#include "color.h"

class Checkerboard : public Texture {
public:
    Checkerboard(Color a, Color b, int rows = 12);
    Color value(double u, double v) const override;

private:
    Color a, b;
    int rows;
};
