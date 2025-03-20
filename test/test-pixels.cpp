#include "pixels.h"
#include "color.h"


// write a function to set all the pixels to the same color
void set_color(Pixels& pixels, Color color) {
    for (int i = 0; i <= pixels.rows; ++i) {
        for (int j = 0; j <= pixels.columns; ++j) {
            if (i == pixels.rows) {// When the end of the column is reached
                i = 0;
                ++j;
                // fill the first pixel
                pixels(i, j) = color;
            }
            else {
                pixels(i,j) = color;
            }
        }
    }
}

// write a function that produces a single color gradient
void gradient(Pixels& pixels) {
    auto color = Black;
    auto gradient_color = color;
    double conv = (1.0/255.0)/10;
    int col = 0;
    for (auto& c : pixels.values) {
        c = Color{color.x += conv, color.y += conv, color.z += conv};
        ++col;
        if (col == pixels.columns) {
            color = gradient_color;
            col = 0;
        }
    }
}

// write a function to color a pretty picture!
void pretty(Pixels& pixels) {
    auto color = Black;
    auto gradient_color = color;
    double conv = (1.0/255.0);
    int col = 0;
    for (auto& c : pixels.values) {
        c = Color{color.x += conv, color.y += conv, color.z += conv};
        ++col;
        if (col == pixels.columns) {
            color = gradient_color;
            col = 0;
        }
    }
    auto color2 = Blue;
    auto gradient_color2 = color2;
    int col2 = 0;
    for (auto& c : pixels.values) {
        if (c != White) {
            c = Color{color2.x += conv, color2.y += conv, color2.z += conv};
            ++col2;
            if (col2 == pixels.rows) {
                color2 = gradient_color2;
                col2 = 0;
            }
        }
    }
}

int main() {
  const unsigned columns = 1280;
  const unsigned rows = 720;
  Pixels pixels{columns, rows};


  // example:
  set_color(pixels, White);
  std::string filename{"white.png"};
  pixels.save_png(filename);
  std::cout << "Wrote: " << filename << '\n';

  // ppm example:
  set_color(pixels, Blue);
  std::string filename2{"blue.ppm"};
  pixels.save_ppm(filename2);
  std::cout << "Wrote: " << filename2 << '\n';

  // gradient example
  gradient(pixels);
  std::string filename3{"Black.ppm"};
  pixels.save_ppm(filename3);
  std::cout << "Wrote: " << filename3 << '\n';

  pretty(pixels);
  std::string filename4{"pretty.ppm"};
  pixels.save_ppm(filename4);
  std::cout << "Wrote: " << filename4 << '\n';
}
