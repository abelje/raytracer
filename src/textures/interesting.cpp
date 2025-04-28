#include "interesting.h"

#include <cmath>

Interesting::Interesting() {}
Color Interesting::value(double u, double v) const {
    // u and v [-0.5, 0.5]
    double x = u - 0.5;
    double y = v - 0.5;

    // convert to polar
    double radius = std::sqrt(x * x + y * y);
    double theta = std::atan2(y, x);

    double swirl = 10*v;
    double r = 0.5*u + 0.5*std::sin(swirl*theta + 10*radius);
    double g = 0.5*v + 0.5*std::cos(swirl*theta - 5*radius);
    double b = 0.5*(u-v) + 0.5*std::sin(15*radius - swirl*theta);
    return {r, g, b};
}