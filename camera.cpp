#include "camera.h"

#include "constants.h"

Camera::Camera(Point3D position, Point3D target, Vector3D up, double fov, double aspect)
    : position{position} {

    // w = unit(Point camera is looking from - point camera is looking at)
    const Vector3D w = unit(position - target);
    const Vector3D u = unit(cross(up,w));
    const Vector3D v = cross(w,u);

    const double theta = fov * (Constants::pi / 180.0);
    const double width = 2 * tan(theta/2);

    // viewport horizontal edge
    horizontal = u * width;

    double height = width/aspect;
    // viewport vertical edge
    vertical = v * height;

    upper_left_corner = position + w - (0.5*(horizontal + vertical));
}

Ray Camera::compute_ray(double s, double t) const {
    //direction = pixel_center - camera center
    Vector3D direction = upper_left_corner - position;
    direction += (horizontal * s);
    direction += (vertical * t);
    // Point3D origin = {position.x * s, 0, position.z * t};
    return {position, direction};
}
