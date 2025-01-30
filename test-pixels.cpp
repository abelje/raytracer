#include "pixels.h"
#include "color.h"


// write a function to set all the pixels to the same color
void set_color(Pixels& pixels, Color color) {

}

// write a function that produces a single color gradient
void gradient(Pixels& pixels) {

}

// write a function to color a pretty picture!
void pretty(Pixels& pixels) {

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
}
