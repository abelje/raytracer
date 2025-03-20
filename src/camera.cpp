#include "camera.h"

#include "constants.h"

Camera::Camera(Point3D position, Point3D target, Vector3D up, double fov, double aspect)
    : position{position} {

    // w = unit(Point camera is looking from - point camera is looking at)
    const Vector3D w = unit(target - position); // target - position
    const Vector3D u = unit(cross(w, up));
    const Vector3D v = cross(w,u);

    const double theta = fov * (Constants::pi / 180.0);
    const double width = 2 * tan(theta/2);

    // viewport horizontal edge
    horizontal = u * width;

    double height = width/aspect;
    // viewport vertical edge
    vertical = v * height;

    upper_left_corner = position + w - (0.5 * horizontal) - (0.5 * vertical);
}

Ray Camera::compute_ray(double s, double t) const {
    Vector3D direction = upper_left_corner + (horizontal * s) + (vertical * t) - position;
    return {position, direction};
}
