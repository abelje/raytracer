#include "ray.h"
#include <iostream>

int main() {
  Ray ray {{0, 0, 0}, {1, 2, 0}};

  std::cout << ray << '\n';
  for (int i = 0; i < 3; ++i) {
    auto P = ray.at(i);
    std::cout << length(P - ray.origin) << '\n';
  }
}
