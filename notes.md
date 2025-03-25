# Raytracer Project
## Generating test files
Right Click on CMake Lists and choose generate CMake.
## Dot Product
This version of the code returns the dot product of a and b.
I learned about dot product more thoroughly on 
[Dot_Product Math_is_fun](https://www.mathsisfun.com/algebra/vectors-dot-product.html)
This project assumes a right-handed coordinate system.
Dot Product creates a shadow projected onto another line.
```c++
double dot(const Vector3D& a, const Vector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
```

If I needed to get the angle, I could do this:
```c++
double dot(const Vector3D& a, const Vector3D& b) {
     double dot_product = a.x * b.x + a.y * b.y + a.z * b.z;
     double dis_a = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
     double dis_b = std::sqrt(b.x * b.x + b.y * b.y + b.z * b.z);
     return 1/cos(dot_product/(dis_a * dis_b));
}
```

## Cross Product
Learned extra information about Cross Product on 
[Cross_Product Math_is_fun](https://www.mathsisfun.com/algebra/vectors-cross-product.html)
```c++
Vector3D cross(const Vector3D& a, const Vector3D& b) {
    double x = a.y * b.z - a.z * b.y;
    double y = a.z * b.x - a.x * b.z;
    double z = a.x * b.y - a.y * b.x;
    return Vector3D{x, y, z};
}
```

## Unit
We have to check if the unit is equal to zero and throw an overflow error if it is, because it is used in division.
```c++
Vector3D unit(const Vector3D& v) {
    if (length(v) == 0) {
        throw std::overflow_error("Unit Length = 0!");
    }
    double b{1/length(v)};
    return Vector3D{b, b, b};
}
```
Test Code:
```c++
SUBCASE( "unit" ) {
    Vector3D a{1, 1, 1};
    double b{1/std::sqrt(3)};
    CHECK( unit(a) == Vector3D{b, b, b} );
    CHECK_THROWS_AS(unit({0, 0, 0}), std::overflow_error);
  }
```
## Making Color Codes
Colors are created using RGB values such as the example of Black below. It creates it using a line in Vector3D.
```c++
const Color Black = {0, 0, 0};
```

## Saving to ppm
Example code: 
```c++
#include <iostream>

int main() {

    // Image

    int image_width = 256;
    int image_height = 256;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}
```
This comes from the Raytracing in one weekend github page.
[Chapter_2](https://raytracing.github.io/books/RayTracingInOneWeekend.html#outputanimage)
I used this code as a base to figure out how to load rgb into my picture.

```c++
void Pixels::save_ppm(const std::string& filename) {
  Pixels pixels(columns, rows);
  Color& color = pixels(rows, columns);
  std::ofstream image(filename);
  for (int j = 0; j < columns; ++j) {
    for (int i = 0; i < rows; ++i) {
      auto x = to_color<unsigned>(color.x);
      auto y = to_color<unsigned>(color.y);
      auto z = to_color<unsigned>(color.z);

      image << x << ' ' << y << ' ' << z << '\n';
    }
  }
  // use to_color<unsigned>(color.x) for printing RGB values to file

  // write the pixel values to a file using the NetBPM ppm image format

}
```

## Saving to png
Example code for lodepng_error_text:
```c++
//Decode from disk to raw pixels with a single function call
void decodeOneStep(const char* filename) {
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;

  //decode
  unsigned error = lodepng::decode(image, width, height, filename);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
}
```
This code is from the documentation at: 
[Example_decode](https://github.com/lvandeve/lodepng/blob/master/examples/example_decode.cpp) \
I used this code to create a basis for how to use the error readout.

## Gradient
Previous idea
```c++
// for (int i = 0, j = 0; i <= pixels.rows && j <= pixels.columns - 1; ++i) {
    //     if (i == pixels.rows) {// When the end of the column is reached
    //         i = 0;
    //         ++j;
    //         color.x += .003;
    //         color.y += .003;
    //         color.z += .003;
    //         // fill the first pixel
    //         pixels(i, j) = color;
    //     }
    //     else {
    //         pixels(i,j) = color;
    //     }
    // }
```

```c++
class A {
public:
    int data
    };
    main
    std::optional<double> maybe_num<
```


Test of std::optional
```c++ 
std::optional<double> maybe_num{1};
  if (maybe_num) {
    std::cout << *maybe_num << '\n'; // maybe_num.value()
  }
  else {
    // std::nullopt
    std::cout << "nothing\n";
  }
```

## Sphere Geometric intersect
```c++
std::optional<double> Sphere::intersect(const Ray& ray) const {
    // if q^2 > r^2 miss
    auto temp = center - ray.origin;
    double temp2 = dot(temp, temp);
    double q2 = -dot(ray.origin, temp) + temp2;
    if (q2 > radius * radius) {
        // std::cout << "miss!\n";
        return std::nullopt;
    }
    if (q2 <= radius * radius || q2 + Constants::epsilon < radius * radius
        || q2 - Constants::epsilon > radius * radius) { // hit
        // v dot (c - p) - h
        // h = sqrt(r^2-q^2)
        return dot(ray.origin, temp) - sqrt(radius * radius - q2);
    }
    else {
        return std::nullopt;
    }
}
```

## Camera Assignment
Create a Camera class that can position the field of view around our objects.

### Compute Ray

Direction was found in Chapter 4.2 of [Raytracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#rays,asimplecamera,andbackground/sendingraysintothescene)
in Listing 9 in the Render section.

```c++
// Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                             - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
// Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
```
This was used to find the direction of the compute_ray function:
```c++
Ray Camera::compute_ray(double s, double t) const {
    //direction = pixel_center - camera center
    Vector3D direction = upper_left_corner - position;
    Point3D origin = {s, position.y, t};
    return {origin, direction};
}
```
For the origin, s and t reflect the amount that the horizontal and vertical position, so I put that in for x and z.

### Camera Constructor

Chapter 12 of [Raytracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#positionablecamera/positioningandorientingthecamera)
helped me to figure out the camera constructor, specifically Listing 82.

```c++
        center = lookfrom;

// Determine viewport dimensions.
        auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
```
My Code:
```c++
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

    upper_left_corner = position + target - (0.5*(horizontal + vertical));
}
```

### Testing the Camera
Given code for tracing rays through a pixel:
```c++
for (int row = 0; row < pixels.rows; ++row) {
    double v = static_cast<double>(row) / (pixels.rows - 1);
    for (int col = 0; col < pixels.columns; ++col) {
        double u = static_cast<double>(col) / (pixels.columns - 1);
        Ray ray = camera.compute_ray(u, v);
        // test intersection with a sphere, then color pixels based on
        // Hit data
```
Recommendation: Place two spheres in the scene and color them based on their surface normals, to orient ourselves in 3D.

Requirements: 
- 3 images where you rotate/translate the camera. Clearly display the values used for each image.
- 2 images where you modify the field of view
Finally finished issues that I had:
- unit() was wrong
- intersect was slightly wrong

### Testing speed of programs
- Download Debian on wsl through command prompt
- Download perf-linux using code:
sudo apt install linux-perf
- Open Clion
- alt + f12 to open terminal
- open a debian terminal (might have to set up a toolchain for it in terminal in settings)
- to test time elapsed use:
(will probably have to use "cd cmake-build-debug" first to change directory) \
perf stat -e cpu-clock ./test-materials.exe

Use perf record ./cmake-build-debug/test-materials-parser.exe  input
perf report for stats

### Making materials
To make a material, you need a name for the material, color, and what it is emitting. The easiest example is diffuse:

## Parser
./cmake-build-debug/test-materials-parser.exe  input

Problem with colors: Too saturated, use numbers from 0 to 1

## UV Mapping (Textures) - 03/21
Texture: (u,v)-> color

pair<double,d> Object::uv(Hit) // Hit has pos, object*, normal

Sphere(u,v) 

Theta = [0, pi] around a circle

## Xoshiro256+
[Original](https://xoshiro.di.unimi.it/xoshiro256plus.c)

## Setting up Object class
To set up the overarching object class, there are some edits that we have to make to the codebase.
```c++
std::optional<Hit> World::find_nearest(const Ray& ray) const {
    double time = Constants::infinity;
    Object* nearest = nullptr;
    for (const auto& object : objects) {
        std::optional<double> t = object->intersect(ray);
        if (t.has_value() && t.value() < time) {
            nearest = object.get();
            time = t.value();
        }
    }
    if (nearest) {
        Hit hit = nearest->construct_hit(ray, time);
        return hit;
    }
    else {
        return {};
    }
}
```

```c++
void Parser::parse_sphere(std::stringstream& ss) {
    Vector3D center;
    double radius;
    std::string material_name;
    if (ss >> center >> radius >> material_name) {
        const Material* material = get_material(material_name);
        std::unique_ptr<Object> object = std::make_unique<Sphere>(center, radius, material);
        world.add(std::move(object));
    }
    else {
        throw std::runtime_error("Malformed sphere");
    }
}
```

```c++
#pragma once

#include "point3d.h"
#include "hit.h"
#include "object.h"
#include <optional>

class Ray;
class Material;

class Sphere : public Object {
public:
    Sphere(const Point3D& center, double radius, const Material* material=nullptr);

    // returns the distance if the ray intersects this sphere, otherwise std::nullopt
    std::optional<double> aintersect(const Ray& ray) const;
    std::optional<double> intersect(const Ray& ray) const override;

    Hit construct_hit(const Ray& ray, double time) const override;

    Point3D center;
    double radius;
};


```

```c++
Sphere::Sphere(const Point3D& center, double radius, const Material* material)
    :Object{material}, center{center}, radius{radius} {}
```