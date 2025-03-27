#include "image.h"

#include "lodepng.h"

Image::Image(const std::string& filename)
    : pixels(filename) {}

Color Image::value(double u, double v) const {
    // map (u, v) to (column, height) on an image (use lodepng)
    // Clamp input texture coordinates to [0,1] x [1,0]
    u = std::clamp(u, 0.0, 1.0);
    v = std::clamp(v, 0.0, 1.0);

    auto row = int(u * pixels.columns);
    auto col = int(v * pixels.rows);

    int index = row * pixels.columns + col;
    auto pixel = pixels.values[index];

    return Color{pixel.x, pixel.y, pixel.z};
}

