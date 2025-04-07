#include "gradient.h"

Gradient::Gradient(Color a, Color b)
    : a{a}, b{b} {}

Color Gradient::value(double u, double v) const {
    // linear color dependence on u and/or v
    // lerp - linear interpolation function
    // you can multiply by u or v to have it do the gradient horizontally or vertically
    // a(1-v^2) + (v^2) * b
    return a + (b-a) * v;
}

