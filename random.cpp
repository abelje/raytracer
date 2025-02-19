#include "random.h"
#include <random>

double random_double() {
    // static means it runs globally once
    static std::random_device rd;
    static std::mt19937 engine{0}; // rd()
    static std::uniform_real_distribution<double> distribution{0, 1};
    return distribution(engine);
}

double random_double(double min, double max) {
    return (max-min) * random_double() + min;
}

Vector3D random_unit_vector() {
    while (true) {
        double x = random_double(-1, 1);
        double y = random_double(-1, 1);
        double z = random_double(-1, 1);
        double length_squared = x*x + y*y + z*z;

        if (1e-160 < length_squared && length_squared <= 1) {
            Vector3D v{x, y, z};
            return v / std::sqrt(length_squared);
        }
    }
}

Vector3D random_hemisphere(const Vector3D& normal) {
    Vector3D v = random_unit_vector();
    if (dot(v, normal) > 0) {
        return v;
    }
    else {
        return -v;
    }
}