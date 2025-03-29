#include "spiral.h"

#include <cmath>
#include "constants.h"

Spiral::Spiral(Color a, Color b, int num_spirals, bool flip)
    : a{a}, b{b}, num_spirals{num_spirals}, flip{flip} {}

Color Spiral::value(double u, double v) const {
    // num_spirals tilts the line up, like a diagonal
    double s = u + num_spirals * v;
    if (flip) {
        // swirl up along the y axis
        s = v + u * num_spirals;
    }
    if (std::sin(s * 2.0 * Constants::pi) > 0) {
        return a;
    }
    else {
        return b;
    }
}