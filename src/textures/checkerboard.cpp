#include "checkerboard.h"
#include <cmath>

#include "constants.h"

Checkerboard::Checkerboard(Color a, Color b, int rows)
    : a{a}, b{b}, rows{rows} {}

Color Checkerboard::value(double u, double v) const {
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

