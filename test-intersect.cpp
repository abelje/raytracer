#include "ray.h"
#include "sphere.h"
#include "constants.h"
#include "pixels.h"
#include "timer.h"
#include "color.h"
#include <iostream>
#include <optional>


int main() {
  // Ray ray1 {{0, 0, 0}, {1, 2, 0}};
  //
  // std::cout << ray1 << '\n';
  // for (int i = 0; i < 3; ++i) {
  //   auto P = ray1.at(i);
  //   std::cout << length(P - ray1.origin) << '\n';
  // }

  Sphere sphere {{0,0,0}, 10};
  // Ray ray1 {{0, 0, 0}, {0,1,0}}; // 1 hit
  Ray ray2 {{0,0,0}, {1,1,1}}; // 2 hits
  std::cout << ray2 << '\n';
  std::optional<double> dis;
  std::optional<double> g_dis;
  double a_elapsed = 0;
  double g_elapsed = 0;
  for (int i = 0; i < 10; ++i) {
    Timer algebraic;
    algebraic.start();
    dis = sphere.aintersect(ray2);
    std::cout << dis.value() << '\n';
    a_elapsed += algebraic.stop();
    // std::cout << "Time: " << a_elapsed << '\n';
    Timer geometric;
    geometric.start();
    g_dis = sphere.intersect(ray2);
    std::cout << "G: " << g_dis.value() << '\n';
    g_elapsed += geometric.stop();
    // std::cout << "Time(g): " << g_elapsed << '\n';
  }
  g_elapsed /= 10;
  std::cout <<"Average Time for geometric: " <<  g_elapsed << '\n';
  a_elapsed /= 10;
  std::cout << "Average Time for algebraic: " <<  a_elapsed << '\n';



  // make an image
   int rows = 720;
   int columns = 1280;
   double scale =0.01;
   Pixels pixels{columns, rows};
   Sphere sphere2 = {{0,3, 0}, 5};
   for (int row = 0; row < rows; ++row) {
     double z = scale * (row - rows/2);
     for (int col = 0; col < columns; ++col) {
       double x = scale * (col - columns/2);
       Ray ray {{x,0,z},{0, 1, 0}};
       auto time = sphere2.intersect(ray);
       if (time) {
           pixels(row, col) = Blue;
       }
     }
   }
  std::string filename{"circle.png"};
  pixels.save_png(filename);
  std::cout << "Wrote: " << filename << '\n';
}
