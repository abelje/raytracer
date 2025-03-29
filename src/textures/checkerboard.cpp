#include "checkerboard.h"
#include <cmath>

#include "constants.h"

Checkerboard::Checkerboard(Color a, Color b)
    : a{a}, b{b} {}

Color Checkerboard::value(double u, double v) const {
    int rows = 12;
    // casting to int makes it available with modulo, std::floor rounds values down
    int iu = (static_cast<int>(std::floor(u * rows)));
    int iv = (static_cast<int>(std::floor(v * rows)));

    if ((iu+iv) % 2 == 0) { // if it is even
        return a;
    }
    else {
        return b;
    }
}

