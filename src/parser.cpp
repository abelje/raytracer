#include "parser.h"
#include "color.h"
#include "diffuse.h"
#include "camera.h"
#include "pixels.h"
#include "sphere.h"
#include "triangle.h"
#include "glass.h"
#include "metallic.h"
#include "lambertian.h"
#include "specular.h"
#include "texture.h"
#include "solid.h"
#include "image.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "gradient.h"
#include "triangle.h"

Parser::Parser(const std::string& filename)
    :filename{filename}, found_camera{false}, found_pixels{false}, found_output{false}, found_rays{false} {
    std::ifstream input{filename};
    if (!input) {
        std::string msg{"Could not open filename: " + filename};
        throw std::runtime_error(msg);
    }
    parse(input);
    verify();
}


Camera Parser::get_camera() {
    return Camera{camera_position, camera_target, camera_up, camera_fov, aspect};
}

Pixels Parser::get_pixels() {
    return Pixels{columns, rows};
}

World Parser::get_world() {
    return std::move(world);
}


void remove_comment(std::string& line) {
    auto i = line.find('#');
    if (i != std::string::npos) { // found a #
        line = line.substr(0, i);
    }
}

bool is_whitespace(const std::string& line) {
    return std::all_of(std::begin(line), std::end(line), ::isspace);
}

void Parser::parse(std::ifstream& input) {
    for (std::string line; std::getline(input, line);) {
        remove_comment(line); // remove all text after #

        if (is_whitespace(line)) {
            continue;
        }

        try {
            std::stringstream ss{line};
            std::string type;
            ss >> type; // gets the first word
            if (type == "material") {
                parse_material(ss);
            }
            else if (type == "camera") {
                parse_camera(ss);
            }
            else if (type == "sphere") {
                parse_sphere(ss);
            }
            else if (type == "triangle") {
                parse_triangle(ss);
            }
            else if (type == "output") {
                parse_output(ss);
            }
            else if (type == "pixels") {
                parse_pixels(ss);
            }
            else if (type == "rays") {
                parse_rays(ss);
            }
            else if (type == "threads") {
                parse_threads(ss);
            }
            else if (type == "texture") {
                parse_texture(ss);
            }
            else {
                throw std::runtime_error("Unknown type: " + type + " in line " + line);
            }

        }
        catch (std::runtime_error& e) {
            std::cout << "Error when parsing line:\n" << line << '\n';
            throw e;
        }

    }
}

void Parser::verify() {
    std::string msg{"Missing from file: "};
    if (!found_camera) {
        throw std::runtime_error(msg + "camera");
    }
    if (!found_pixels) {
        throw std::runtime_error(msg + "pixels");
    }
    if (world.objects.empty()) {
        throw std::runtime_error(msg + "sphere");
    }
    if (filename.empty()) {
        throw std::runtime_error(msg + "output");
    }
    if (materials.empty()) {
        throw std::runtime_error(msg + "material");
    }
    if (!found_rays) {
        throw std::runtime_error(msg + "rays");
    }
}

void Parser::parse_material(std::stringstream& ss) {
    std::string name, kind;
    std::string texture_name;
    bool emitting;

    ss >> name >> kind >> texture_name >> std::boolalpha >> emitting;
    Texture* texture = get_texture(texture_name);

    if (kind == "diffuse") {
        materials[name] = std::make_unique<Diffuse>(texture, emitting);
    }
    else if (kind == "lambertian") {
        materials[name] = std::make_unique<Lambertian>(texture, emitting);
    }
    else if (kind == "specular") {
        materials[name] = std::make_unique<Specular>(texture, emitting);
    }
    else if (kind == "metallic") {
        double fuzz;
        if (ss >> fuzz) {
            materials[name] = std::make_unique<Metallic>(texture, emitting, fuzz);
        }
        else {
            throw std::runtime_error("Missing fuzz parameter for metal");
        }
    }
    else if (kind == "glass") {
        materials[name] = std::make_unique<Glass>(texture, emitting);
    }
    else {
        throw std::runtime_error("Unknown material: " + kind);
    }
}

Material* Parser::get_material(const std::string& material) {
    auto i = materials.find(material);
    if (i != materials.end()) { // found it!
        return i->second.get();
    }
    else {
        throw std::runtime_error("Unknown material: " + material);
    }
}

void Parser::parse_texture(std::stringstream& ss) {
    std::string name, kind;
    ss >> name >> kind;
    if (kind == "solid") {
        Color color;
        if (ss >> color) {
            textures[name] = std::make_unique<Solid>(color);
        }
        else {
            throw std::runtime_error("Missing color for " + kind + " texture: " + name);
        }
    }
    else if (kind == "gradient") {
        Color a, b;
        if (ss >> a && ss >> b) {
            textures[name] = std::make_unique<Gradient>(a, b);
        }
        else {
            throw std::runtime_error("Missing color for " + kind + " texture: " + name + "(There are 2 colors)");
        }
    }
    else if (kind == "image") {
        std::string filename;
        if (ss >> filename) {
            textures[name] = std::make_unique<Image>(filename);
        }
        else {
            throw std::runtime_error("Missing filename for " + kind + " texture: " + name);
        }
    }
    else if (kind == "checkerboard") {

    }
    else if (kind == "normal") {

    }
    else if (kind == "other") {

    }
    // others: image, checkerboard, etc
}

Texture* Parser::get_texture(const std::string& texture) {
    auto i = textures.find(texture);
    if (i != textures.end()) { // found it!
        return i->second.get();
    }
    else {
        throw std::runtime_error("Unknown texture: " + texture);
    }
}

void Parser::parse_sphere(std::stringstream& ss) {
    Vector3D center;
    double radius;
    std::string material_name;
    if (ss >> center >> radius >> material_name) {
        const Material* material = get_material(material_name);
        std::unique_ptr<Object> object = std::make_unique<Sphere>(center, radius, material);
        world.add(std::move(object));
    }
    else {
        throw std::runtime_error("Malformed sphere");
    }
}

void Parser::parse_triangle(std::stringstream& ss) {
    Point3D vertex0, vertex1, vertex2;
    std::string material_name;
    if (ss >> vertex0 >> vertex1 >> vertex2 >> material_name) {
        const Material* material = get_material(material_name);
        std::unique_ptr<Object> object = std::make_unique<Triangle>(vertex0, vertex1, vertex2, material);
        world.add(std::move(object));
    }
}

void Parser::parse_camera(std::stringstream& ss) {
    if (ss >> camera_position >> camera_target >> camera_up >> camera_fov) {
        found_camera = true;
    }
    else {
        throw std::runtime_error("Malformed camera");
    }

}
void Parser::parse_rays(std::stringstream& ss) {
    if (ss >> ray_depth >> ray_samples) {
        found_rays = true;
    }

}
void Parser::parse_pixels(std::stringstream& ss) {
    if (ss >> columns >> rows) {
        aspect = static_cast<double>(columns) / rows;
        found_pixels = true;
    }

}
void Parser::parse_output(std::stringstream& ss) {
    if (ss >> filename) {
        found_output = true;
    }
}

void Parser::parse_threads(std::stringstream& ss) {
    if (!(ss>>num_threads)) {
        throw std::runtime_error("Missing number of threads");
    }
}
