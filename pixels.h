#pragma once

#include "color.h"
#include <vector>

//TODO: Implement these functions
class Pixels {
public:
    Pixels(int columns, int rows);

    // get pixel value
    const Color& operator()(int row, int col) const;

    // set pixel value
    Color& operator()(int row, int col);

    // saving files
    void save_ppm(const std::string& filename);
    void save_png(const std::string& filename);

    const int columns, rows;
    std::vector<Color> values;
};

