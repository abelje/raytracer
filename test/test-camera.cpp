#include "camera.h"
#include "sphere.h"
#include "pixels.h"
#include "world.h"

Pixels generate_spheres(const Sphere& sphere, const Sphere& ground, const Camera& camera, Pixels& pixels);
void generate_image(const std::string& filename, Pixels& pixels);

int main() {
    // create spheres, one is small sphere, the other is the ground
    Sphere ground{{0, 0, -100}, 100};
    Sphere sphere{{0,0, 0}, 1};

    //image size
    Pixels pixels {1280, 720};

    //create the camera
    Vector3D position{0, -10, 0}, up{0,0,1};
    Vector3D target = sphere.center;
    double aspect_ratio = static_cast<double>(pixels.columns)/pixels.rows;
    double fov = 90;

    //test 1 camera
    Camera camera{position, target, up, fov, aspect_ratio};

    generate_spheres(sphere, ground, camera, pixels);
    generate_image("test.png", pixels);

    // fov25 pixels
    Pixels pixels25 {1280, 720};
    //fov25 camera same as last except for a changed fov, used for tests of 10 and 180 fov
    fov = 25;
    camera= {position, target, up, fov, aspect_ratio};

    generate_spheres(sphere, ground, camera, pixels25);
    generate_image("FOV25.png", pixels25);

    Pixels pixels125 {1280, 720};
    fov = 125;
    camera= {position, target, up, fov, aspect_ratio};
    generate_spheres(sphere, ground, camera, pixels125);
    generate_image("FOV125.png", pixels125);

    //translate -20 downwards from the original test image, otherwise same as test.png
    Pixels pixels_trans20 {1280, 720};
    position = {0, -10, -20};
    camera = {position, target, up, fov, aspect_ratio};

    generate_spheres(sphere, ground, camera, pixels_trans20);
    generate_image("translate20.png", pixels_trans20);

    // Translate 10 to the right
    Pixels pixels_trans10 {1280, 720};
    position = {10, -10, 0};
    camera = {position, target, up, fov, aspect_ratio};

    generate_spheres(sphere, ground, camera, pixels_trans10);
    generate_image("translate10.png", pixels_trans10);

    // Rotate to the x axis
    Pixels pixels_rotate {1280, 720};
    up = {1,0,0};
    camera = {position, target, up, fov, aspect_ratio};
    generate_spheres(sphere, ground, camera, pixels_rotate);
    generate_image("rotate.png", pixels_rotate);
}

Pixels generate_spheres(const Sphere& sphere, const Sphere& ground, const Camera& camera, Pixels& pixels) {
    for (int row = 0; row < pixels.rows; ++row) {
        double v = static_cast<double>(row) / (pixels.rows - 1);
        for (int col = 0; col < pixels.columns; ++col) {
            double u = static_cast<double>(col) / (pixels.columns - 1);
            Ray ray = camera.compute_ray(u, v);
            // test intersection with a sphere, then color pixels based on Hit data
            std::optional<double> stime = sphere.intersect(ray);
            std::optional<double> gtime = ground.intersect(ray);

            if (gtime) {
                Hit hit2 = ground.construct_hit(ray, gtime.value());
                pixels(row, col) = hit2.normal;
            }

            if (stime) {
                Hit hit = sphere.construct_hit(ray, stime.value());
                pixels(row, col) = hit.normal;
            }
        }
    }
    return pixels;
}

void generate_image(const std::string& filename, Pixels& pixels) {
    pixels.save_png(filename);
    std::cout << "Wrote: " << filename << '\n';
}