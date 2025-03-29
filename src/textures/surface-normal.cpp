#include "surface-normal.h"

#include "constants.h"
#include "object.h"
#include "hit.h"

Surface_Normal::Surface_Normal(bool smooth)
    : smooth(smooth) {}

Color Surface_Normal::value(double u, double v) const {
    // double theta = std::acos(hit.normal.z); // [0, pi]
    // double phi = std::atan2(hit.normal.y, hit.normal.x); // [-pi, pi]
    // double u = 0.5 + phi / (2 * Constants::pi); // [0, 1]
    // double v = theta / Constants::pi; // [0, 1]
    double theta = v * Constants::pi;
    double phi = (u - 0.5) * 2 * Constants::pi;

    double x = std::sin(theta) * std::cos(phi);
    double y = std::sin(theta) * std::sin(phi);
    double z = std::cos(theta);

    Vector3D normal = Vector3D(x, y, z);
    if (smooth) {
        // The next two lines bring it back into the color spectrum, otherwise it will generate a plus like the original tests
        normal += {1, 1, 1};
        normal *= 0.5;
    }
    return Color{normal.x, normal.y, normal.z};

}
