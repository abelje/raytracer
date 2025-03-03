#include <iomanip>

#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "constants.h"
#include "pixels.h"
#include "ray.h"
#include "hit.h"
#include "random.h"
#include "world.h"
#include "diffuse.h"
#include "glass.h"
#include "lambertian.h"
#include "specular.h"
#include "metallic.h"


Color trace(const World& world, const Ray& ray);
Color trace_path(const World& world, const Ray& ray, int depth);
void print_progress(long long ray_num, long long total_rays);

void in_class();
void test_all();

int main() {
    // in_class();

    // materials
    Diffuse ground {Gray, false};
    Diffuse blue{Blue, false};
    Lambertian lime{Lime, false};
    Specular purple{Purple, false};
    Metallic bronze{Orange, false, 0.5};
    Diffuse light{White, true};

    // world
    World world;
    world.add({0, 0, 0.5}, 0.3, &blue);
    world.add({2, 0.5, 0.5}, 0.3, &lime);
    world.add({2, -1, 0.5}, 0.3, &purple);
    world.add({2.75, -0.5, 0.5}, 0.3, &bronze);
    world.add({0,0, -100}, 100, &ground);
    world.add({0, 0, 0}, 100, &light);

    // specify the number of pixels
    Pixels pixels{1280, 720};

    // Set up camera
    Vector3D position{10, -2, 3}, up{0, 0, 1};
    Vector3D target{0, 0, 0};
    double fov{20};
    double aspect = static_cast<double>(pixels.columns) / pixels.rows;
    Camera camera{position, target, up, fov, aspect};

    constexpr int samples = 10000;
    constexpr int ray_depth = 5;

    const long long rays_total = pixels.rows* pixels.columns * static_cast<long long>(samples);
    long long ray_num = 0;

    for (auto row = 0; row < pixels.rows; ++row) {
        for (auto col = 0; col < pixels.columns; ++col) {
            for (int N = 0; N < samples; ++N) {
                double y = (row + random_double()) / (pixels.rows - 1);
                double x = (col + random_double()) / (pixels.columns - 1);
                // cast samples number of rays
                Ray ray = camera.compute_ray(x, y);
                pixels(row, col) += trace_path(world, ray, ray_depth);

                ++ray_num;
                if (ray_num % (rays_total / 100) == 0) {
                    print_progress(ray_num, rays_total);
                }
            }
            pixels(row, col) /= samples;
        }
    }

    std::string filename{"spheres-test.png"};
    pixels.save_png(filename);
    std::cout << "Wrote " << filename << '\n';
}

Color trace(const World& world, const Ray& ray) {
    std::optional<Hit> hit = world.find_nearest(ray);
    if (hit.has_value()) {
        return hit->normal;
    }
    else {
        return Black;
    }
}

Color trace_path(const World& world, const Ray& ray, int depth) {
    if (depth == 0) {
        return Black;
    }

    std::optional<Hit> hit = world.find_nearest(ray);
    if (!hit) {
        return Blue; //out of color palatte, so its an obvious edge case
        // artifacts?
    }

    const Material* material = hit->sphere->material;
    if (material->emitting) {
        return material->color;
    }

    //more bounces!
    Ray scattered = material->scatter(ray, hit.value());
    return material->color * trace_path(world, scattered, depth-1);
}

void print_progress(long long ray_num, long long total_rays) {
    auto width = std::to_string(total_rays).length() + 4;
    int percentage = std::round(static_cast<double>(ray_num) / total_rays * 100);

    std::cout << "\rProgram:" << std::setw(3) << percentage << "%";
    std::cout << std::setw(width) << ray_num << '/' << total_rays << " rays";
    std::cout << std::flush;
}

void in_class() {
    // materials
    Diffuse red{{1, 0, 0}, false};
    Diffuse gray{{0.5, 0.5, 0.5}, false};
    Diffuse light{{1, 1, 1}, true};
    Glass glass {{1,1,1}, false};

    // world
    World world;
    world.add({0, 0, 0}, 0.3, &glass);
    world.add({1, 0, 0}, 0.3, &red);
    world.add({0,0, -100}, 100, &gray);
    //world.add({1200, 1100, 0}, 1000, &light);
    world.add({0, 0, 0}, 100, &light);


    // specify the number of pixels
    Pixels pixels{1280, 720};

    // create the camera
    Vector3D position{10, -2, 3}, up{0, 0, 1};
    Vector3D target{0, 0, 0};
    double fov{20};
    double aspect = static_cast<double>(pixels.columns) / pixels.rows;
    Camera camera{position, target, up, fov, aspect};

    constexpr int samples = 1000;
    constexpr int ray_depth = 5;


    const long long rays_total = pixels.rows* pixels.columns * static_cast<long long>(samples);
    long long ray_num = 0;

    for (auto row = 0; row < pixels.rows; ++row) {
        for (auto col = 0; col < pixels.columns; ++col) {
            for (int N = 0; N < samples; ++N) {
                double y = (row + random_double()) / (pixels.rows - 1);
                double x = (col + random_double()) / (pixels.columns - 1);
                // cast samples number of rays
                Ray ray = camera.compute_ray(x, y);
                pixels(row, col) += trace_path(world, ray, ray_depth);

                ++ray_num;
                if (ray_num % (rays_total / 100) == 0) {
                    print_progress(ray_num, rays_total);
                }
            }
            pixels(row, col) /= samples;
        }
    }
    std::string filename{"sphere1.png"};
    pixels.save_png(filename);
    std::cout << "Wrote " << filename << '\n';
}