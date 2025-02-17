#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "constants.h"
#include "pixels.h"
#include "ray.h"
#include "hit.h"
#include "world.h"


Color trace(const World& world, const Ray& ray);

int main() {
    // world
    World world;
    world.add({0, 0, 0}, 0.3);
    world.add({1, 0, 0}, 0.3);

  
    // specify the number of pixels
    Pixels pixels{1280, 720};

    // create the camera
    Vector3D position{10, -2, 3}, up{0, 0, 1};
    Vector3D target{0, 0, 0};
    double fov{20};
    double aspect = static_cast<double>(pixels.columns) / pixels.rows;
    Camera camera{position, target, up, fov, aspect};
  
    for (auto row = 0; row < pixels.rows; ++row) {
	double y = static_cast<double>(row) / (pixels.rows - 1);
	for (auto col = 0; col < pixels.columns; ++col) {
	    double x = static_cast<double>(col) / (pixels.columns - 1);
	    Ray ray = camera.compute_ray(x, y);
	    pixels(row, col) = trace(world, ray);
	}
    }
    std::string filename{"sphere.png"};
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