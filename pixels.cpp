#include "pixels.h"
#include "lodepng.h"
#include <fstream>
#include <cmath>

Pixels::Pixels(int columns, int rows)
  :columns{columns}, rows{rows}, values(columns*rows) {}

const Color& Pixels::operator()(int row, int col) const {
  // TODO: implement me
  return values[row*columns + col];
}
Color& Pixels::operator()(int row, int col) {
  // TODO: implement me
  return values[row*columns + col];
}

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

void Pixels::save_png(const std::string& filename) {
  // lodepng expects pixels to be in a vector of unsigned char, where
  // the elements are R, G, B, alpha, R, G, B, alpha, etc.  Use
  // to_color<unsigned char>(color.x) for putting colors in this
  // vector, and lodepng::encode(filename, vector, width, height)
  // (make sure you check the error code returned by this function
  // using lodepng_error_text(unsigned error)).
  Pixels pixels(columns, rows);
  Color& color = pixels(rows, columns);
  std::ofstream image(filename);
  std::vector<unsigned char> lode;
  for (int j = 0; j < columns; ++j) {
    for (int i = 0; i < rows; ++i) {
      auto x = to_color<unsigned char>(color.x);
      lode.push_back(x);
      auto y = to_color<unsigned char>(color.y);
      lode.push_back(y);
      auto z = to_color<unsigned char>(color.z);
      lode.push_back(z);
      unsigned error = lodepng::encode(filename, lode, i, j);
      if (error) {
        std::cout << "Decoder error: " << lodepng_error_text(error);
      }
    }
  }
  for (auto i : lode) {
    image << i << ' ';
  }
}

double gamma_correction(double value) {
  // double gamma = 2.2;
  double gamma = 1.0;
  return std::pow(value, 1/gamma);
}
