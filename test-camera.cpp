#include "camera.h"
#include "sphere.h"
#include "pixels.h"
#include "world.h"

Pixels generate_spheres(const Sphere& sphere, const Sphere& ground, const Camera& camera, Pixels& pixels);

int main() {
    // create spheres, one is small sphere, the other is the ground
    Sphere ground{{0, 0, -25}, 26};
    Sphere sphere{{0,0, 0}, 1};

    //image size
    Pixels pixels {1280, 720};

    //create the camera
    Vector3D position{0, -5, 0}, up{0,0,1};
    Vector3D target{0, 0, 0};
    double aspect_ratio = static_cast<double>(pixels.columns)/pixels.rows;

    //test 1 camera
    Camera camera{position, target, up, 90, aspect_ratio};

    // World world;
    // world.add(sphere.center,  sphere.radius);
    // world.add(ground.center,  ground.radius);

    for (int row = 0; row < pixels.rows; ++row) {
        double v = static_cast<double>(row) / (pixels.rows - 1);
        for (int col = 0; col < pixels.columns; ++col) {
            double u = static_cast<double>(col) / (pixels.columns - 1);
            Ray ray = camera.compute_ray(u, v);
            // Ray ray2 = camera.compute_ray(u, v);
            // test intersection with a sphere, then color pixels based on Hit data
            // std::optional<double> time = sphere.intersect(ray);
            std::optional<double> time2 = ground.intersect(ray);

            if (time2) {
                Hit hit2 = ground.construct_hit(ray, time2.value());
                double shading = std::abs(dot(ray.direction, hit2.normal));
                // hit2.normal.x *= shading;
                hit2.normal.y *= shading;
                pixels(row, col) = hit2.normal;
            }

            // if (time) {
            //     // std::cout << "1";
            //     Hit hit = sphere.construct_hit(ray2, time.value());
            //     pixels(row, col) = hit.normal;
            // }

        }
    }

    for (int row = 0; row < pixels.rows; ++row) {
        double v = static_cast<double>(row) / (pixels.rows - 1);
        for (int col = 0; col < pixels.columns; ++col) {
            double u = static_cast<double>(col) / (pixels.columns - 1);
            Ray ray = camera.compute_ray(u, v);
            Ray ray2 = camera.compute_ray(u, v);
            // test intersection with a sphere, then color pixels based on Hit data
            std::optional<double> time = sphere.intersect(ray);
            // std::optional<double> time2 = ground.intersect(ray);

            // if (time2) {
            //     Hit hit2 = ground.construct_hit(ray, time2.value());
            //     double shading = std::abs(dot(ray.direction, hit2.normal));
            //     // hit2.normal.x *= shading;
            //     hit2.normal.y *= shading;
            //     pixels(row, col) = hit2.normal;
            // }

            if (time) {
                // std::cout << "1";
                Hit hit = sphere.construct_hit(ray2, time.value());
                pixels(row, col) = hit.normal;
            }

        }
    }



    std::string filename = "test.png";
    pixels.save_png(filename);
    std::cout << "Wrote: " << filename << '\n';

    // fov10 pixels
    // Pixels pixels10 {1280, 720};
    //fov10 camera same as last except for a changed fov, used for tests of 10 and 180 fov
    // Camera camera2{position, target, up, 10, aspect_ratio};

    // generate_spheres(sphere, ground, camera2, pixels10);

    // std::string filename2 = "test2.png";
    // pixels10.save_png(filename2);
    // std::cout << "Wrote: " << filename2 << '\n';

    // //translate -20 from the original test image, otherwise same as test.png
    // Pixels pixels_trans {1280, 720};
    // position = {0, -10, -30};
    // Camera camera2{position, target, up, 45, aspect_ratio};
    //
    // // rotate the camera by 20 from the test image
    // // TargetCenter.png
    // // Pixels pixels_rot {1280, 720};
    // // position = {0,-10, -10};
    // // target = {-10, 0, -10};
    // // Camera camera2{position, target, up, 45, aspect_ratio};
    //
    //
    //
    // for (int row = 0; row < pixels_trans.rows; ++row) {
    //     double v = static_cast<double>(row) / (pixels.rows - 1);
    //     for (int col = 0; col < pixels_trans.columns; ++col) {
    //         double u = static_cast<double>(col) / (pixels.columns - 1);
    //         Ray ray = camera2.compute_ray(u, v);
    //         // test intersection with a sphere, then color pixels based on Hit data
    //         std::optional<double> time = sphere.intersect(ray);
    //         std::optional<double> time2 = ground.intersect(ray);
    //
    //         if (time2) {
    //             Hit hit = ground.construct_hit(ray, time2.value());
    //             pixels_trans(row, col) = hit.normal;
    //         }
    //         if (time) {
    //             Hit hit = sphere.construct_hit(ray, time.value());
    //             pixels_trans(row, col) = hit.normal;
    //         }
    //     }
    // }
    //
    // std::string filename2 = "translate20.png";
    // pixels_trans.save_png(filename2);
    // std::cout << "Wrote: " << filename2 << '\n';
}

Pixels generate_spheres(const Sphere& sphere, const Sphere& ground, const Camera& camera, Pixels& pixels) {
    for (int row = 0; row < pixels.rows; ++row) {
        double v = static_cast<double>(row) / (pixels.rows - 1);
        for (int col = 0; col < pixels.columns; ++col) {
            double u = static_cast<double>(col) / (pixels.columns - 1);
            Ray ray = camera.compute_ray(u, v);
            // test intersection with a sphere, then color pixels based on Hit data
            std::optional<double> time = sphere.intersect(ray);
            std::optional<double> time2 = ground.intersect(ray);
            if (time2) {
                Hit hit = ground.construct_hit(ray, time2.value());
                pixels(row, col) = hit.normal;
            }
            if (time) {
                Hit hit = sphere.construct_hit(ray, time.value());
                pixels(row, col) = hit.normal;
            }
        }
    }
    return pixels;
}