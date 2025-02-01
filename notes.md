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