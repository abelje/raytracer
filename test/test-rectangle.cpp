#include "ray.h"
#include "rectangle.h"
#include "pixels.h"
#include "diffuse.h"

int main() {
    //needs a material
    Rectangle t{{0, 0, 0}, {5, 0, 0}, 5, nullptr};

    int rows = 720;
    int columns = 1280;
    double scale =0.05;
    Pixels pixels{columns, rows};

    for (int row = 0; row < rows; ++row) {
        double z = scale * (row - rows/2);
        for (int col = 0; col < columns; ++col) {
            double x = scale * (col - columns/2);
            Ray ray {{x,-2,z},{0, 1, 0}};
            auto time = t.intersect(ray);
            if (time) {
                pixels(row, col) = {1, 1, 1};
            }
        }
    }
    std::string filename{"rectangle.png"};
    pixels.save_png(filename);
    std::cout << "Wrote: " << filename << '\n';
}
