#pragma once

#include <string>

class Camera;
class Pixels;
class World;

class Parser {
public:
    Parser(const std::string& filename);

    Camera get_camera();
    Pixels get_pixels();
    World get_world();

    std::string filename;
    int ray_depth, ray_samples;

private:

    void parse(std::ifstream& input);
    void verify();

    void parse_material(std::stringstream& ss);
};