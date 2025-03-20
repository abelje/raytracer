#include <iostream>
#include <fstream>
#include "camera.h"  // Include your Camera class
#include "ray.h"     // Include Ray class if it's in a separate file
#include "vector3d.h" // Include necessary vector operations
#include "pixels.h"
#include "hit.h"
#include "sphere.h"

int main() {
    Sphere sphere {{0,0,0}, 1};

    //image size
    Pixels pixels {1280, 720};

    //create the camera
    Vector3D position{0, -10, 0}, up{0,0,1};
    Vector3D target = sphere.center;
    double aspect_ratio = static_cast<double>(pixels.columns)/pixels.rows;

    //test 1 camera
    Camera camera{position, target, up, 90, aspect_ratio};
    double v = static_cast<double>(pixels.rows/2.0) / (pixels.rows - 1);
    double u = static_cast<double>(pixels.columns/2.0) / (pixels.columns - 1);
    Ray ray = camera.compute_ray(0.5, 0.5);
    std::cout << ray << '\n';
    std::cout << "Center: " << sphere.center << '\n';
    std::optional<double> g_dis;

    g_dis = sphere.intersect(ray);
    if (g_dis) {
        std::cout << g_dis.value() << '\n';
    }

}