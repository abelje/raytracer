#include "camera.h"
#include "sphere.h"
#include "pixels.h"

int main() {
    // create spheres, one is small sphere, the other is the ground
    Sphere ground{{0, 0, -100}, 100};
    Sphere sphere{{0,0,1}, 1};

    //image size
    Pixels pixels {1280, 720};

    //create the camera
    Vector3D position{0, -10, 10}, up{0,0,1};
    Vector3D target{0,0,0};
    double aspect_ratio = pixels.columns/pixels.rows;

    Camera camera{position, target, up, 45, aspect_ratio};

    for (int row = 0; row < pixels.rows; ++row) {
        double v = static_cast<double>(row) / (pixels.rows - 1);
        for (int col = 0; col < pixels.columns; ++col) {
            double u = static_cast<double>(col) / (pixels.columns - 1);
            Ray ray = camera.compute_ray(u, v);
            // test intersection with a sphere, then color pixels based on Hit data
            std::optional<double> time = sphere.intersect(ray);
            if (time) {
                Hit hit = sphere.construct_hit(ray, time.value());
                pixels(row, col) = hit.normal;
            }

            std::optional<double> time2 = ground.intersect(ray);
            if (time2) {
                Hit hit = ground.construct_hit(ray, time2.value());
                pixels(row, col) = hit.normal;
            }
            // else {
            //     pixels(row, col) = Blue;
            // }
        }
    }

    std::string filename = "test.png";
    pixels.save_png(filename);
    std::cout << "Wrote: " << filename << '\n';
}