#include "pixels.h"
#include "lodepng.h"
#include <fstream>
#include <cmath>

Pixels::Pixels(int columns, int rows)
  :columns{columns}, rows{rows}, values(columns*rows) {}

const Color& Pixels::operator()(int row, int col) const {
  return values[row*columns + col];
}
Color& Pixels::operator()(int row, int col) {
  return values[row*columns + col];
}

void Pixels::save_ppm(const std::string& filename) {
  std::ofstream output(filename);
  if (!output) {
    throw std::runtime_error("Cannot open output file: " + filename);
  }
  //Print headers
  output << "P3" << '\n' << "1280 720" << '\n' << "255\n";

  for (auto& c : values) {
    output << to_color<unsigned>(c.x) << ' ' << to_color<unsigned>(c.y) << ' ' << to_color<unsigned>(c.z) << '\n';
  }
  // use to_color<unsigned>(color.x) for printing RGB values to file

  // write the pixel values to a file using the NetBPM ppm image format

}

void Pixels::save_png(const std::string& filename) {
  std::ofstream output(filename);
  if(!output) {
    throw std::runtime_error("Cannot open output file: " + filename);
  }
  std::vector<unsigned char> data;
  for (auto c : values) {
    data.push_back(to_color<unsigned char>(c.x));
    data.push_back(to_color<unsigned char>(c.y));
    data.push_back(to_color<unsigned char>(c.z));
    data.push_back(255);
  }
  unsigned error = lodepng::encode(filename, data, columns,  rows);
  if (error) {
    throw std::runtime_error(lodepng_error_text(error));
  }
}


double gamma_correction(double value) {
  // double gamma = 2.2;
  double gamma = 1.0;
  return std::pow(value, 1/gamma);
}
