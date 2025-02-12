#pragma once

#include "point3d.h"
#include "vector3d.h"
#include "ray.h"

class Camera {
public:
    Camera(Point3D position, Point3D target, Vector3D up, double fov, double aspect);

    // s, t are normalized vectors for horizontal, vertical directions
    // s,t in [0,1]
    Ray compute_ray(double s, double t) const;

private:
    // position is where the camera is located
    // upper_left_corner is the top left corner of the viewport
    Point3D position, upper_left_corner;
    // the horizontal and vertical sides of the viewport
    Vector3D horizontal, vertical;
};
