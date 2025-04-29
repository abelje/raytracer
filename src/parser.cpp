#include "parser.h"
#include "color.h"
#include "diffuse.h"
#include "camera.h"
#include "pixels.h"
#include "sphere.h"
#include "triangle.h"
#include "rectangle.h"
#include "glass.h"
#include "metallic.h"
#include "lambertian.h"
#include "specular.h"
#include "texture.h"
#include "solid.h"
#include "image.h"
#include "gradient.h"
#include "checkerboard.h"
#include "surface-normal.h"
#include "spiral.h"
#include "isotropic.h"
#include "marble.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "constant_medium.h"


Parser::Parser(const std::string& filename)
    :filename{filename}, found_camera{false}, found_pixels{false}, found_output{false}, found_rays{false} {
    std::ifstream input{filename};
    if (!input) {
        std::string msg{"Could not open filename: " + filename};
        throw std::runtime_error(msg);
    }
    set_parse_map();
    setup_parse_material();
    setup_parse_texture();
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

void Parser::set_parse_map() {
    parse_map["material"] = [this](std::stringstream& ss) {parse_material(ss);};
    parse_map["camera"] = [this](std::stringstream& ss) {parse_camera(ss);};
    parse_map["sphere"] = [this](std::stringstream& ss) {parse_sphere(ss);};
    parse_map["triangle"] = [this](std::stringstream& ss) {parse_triangle(ss);};
    parse_map["rectangle"] = [this](std::stringstream& ss) {parse_rectangle(ss);};
    parse_map["constant_medium"] = [this](std::stringstream& ss) {parse_constant_medium(ss);};
    parse_map["mesh"] = [this](std::stringstream& ss) {parse_mesh(ss);};
    parse_map["output"] = [this](std::stringstream& ss) {parse_output(ss);};
    parse_map["pixels"] = [this](std::stringstream& ss) {parse_pixels(ss);};
    parse_map["rays"] = [this](std::stringstream& ss) {parse_rays(ss);};
    parse_map["threads"] = [this](std::stringstream& ss) {parse_threads(ss);};
    parse_map["texture"] = [this](std::stringstream& ss) {parse_texture(ss);};
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

            auto function = parse_map.find(type);
            if (function != parse_map.end()) {
                function->second(ss);
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

void Parser::setup_parse_material() {
    material_map["diffuse"] = [](Texture* texture, bool emitting, std::stringstream&) {
        return std::make_unique<Diffuse>(texture, emitting);
    };
    material_map["lambertian"] = [](Texture* texture, bool emitting, std::stringstream&) {
        return std::make_unique<Lambertian>(texture, emitting);
    };
    material_map["specular"] = [](Texture* texture, bool emitting, std::stringstream&) {
        return std::make_unique<Specular>(texture, emitting);
    };
    material_map["metallic"] = [](Texture* texture, bool emitting, std::stringstream& ss) {
        double fuzz;
        if (ss >> fuzz) {
                return std::make_unique<Metallic>(texture, emitting, fuzz);
        }
        else {
                throw std::runtime_error("Missing fuzz parameter for metal");
        }
    };
    material_map["glass"] = [](Texture* texture, bool emitting, std::stringstream&) {
        return std::make_unique<Glass>(texture, emitting);
    };
    material_map["isotropic"] = [](Texture* texture, bool emitting, std::stringstream&) {
        return std::make_unique<Isotropic>(texture, emitting);
    };
}

void Parser::parse_material(std::stringstream& ss) {
    std::string name, kind;
    std::string texture_name;
    bool emitting;

    ss >> name >> kind >> texture_name >> std::boolalpha >> emitting;
    Texture* texture = get_texture(texture_name);

    auto function = material_map.find(kind);
    if (function != material_map.end()) {
        materials[name] = function->second(texture, emitting, ss);
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

void Parser::setup_parse_texture() {
    texture_map["solid"] = [](std::stringstream& ss) {
        Color color;
        if (!(ss >> color)) {
            throw std::runtime_error("Missing color for solid texture: ");
        }
        return std::make_unique<Solid>(color);
    };
    texture_map["gradient"] = [](std::stringstream& ss) {
        Color a, b;
        if (!(ss >> a && ss >> b)) {
            throw std::runtime_error("Missing color for gradient texture (There are 2 colors): ");
        }
        return std::make_unique<Gradient>(a, b);
    };
    texture_map["image"] = [](std::stringstream& ss) {
        std::string filename;
        if (!(ss >> filename)) {
            throw std::runtime_error("Missing filename for image texture: ");
        }
        return std::make_unique<Image>(filename);
    };
    texture_map["checkerboard"] = [](std::stringstream& ss) {
        Color a, b;
        int rows;
        if (!(ss >> a && ss >> b)) {
            throw std::runtime_error("Missing color(s) for checkerboard texture: ");
        }
        if (ss >> rows) {
            return std::make_unique<Checkerboard>(a, b, rows);
        }
        else {
            return std::make_unique<Checkerboard>(a, b);
        }
    };

    texture_map["normal"] = [](std::stringstream& ss) {
        bool smooth;
        if (ss >> std::boolalpha >> smooth) {
            return std::make_unique<Surface_Normal>(smooth);
        }
        else {
            return std::make_unique<Surface_Normal>();
        }
    };
    texture_map["spiral"] = [](std::stringstream& ss) {
        Color a, b;
        int num_spirals;
        bool flip;
        if (!(ss >> a && ss >> b && ss >> num_spirals)) {
            throw std::runtime_error("Missing colors or spirals for spiral texture: ");
        }
        if (ss >> std::boolalpha >> flip) {
            return std::make_unique<Spiral>(a, b, num_spirals, flip);
        }
        else {
            return std::make_unique<Spiral>(a, b, num_spirals);
        }
    };
    texture_map["marble"] = [](std::stringstream&){
        return std::make_unique<Marble>();
    };
}

void Parser::parse_texture(std::stringstream& ss) {
    std::string name, kind;
    ss >> name >> kind;
    auto function = texture_map.find(kind);
    if (function != texture_map.end()) {
        try {
            textures[name] = function->second(ss);
        }
        catch (std::runtime_error& e) {
            throw std::runtime_error(e.what() + name);
        }
    }
    else {
        throw std::runtime_error("Unknown " + kind + "texture: " + name);
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
        throw std::runtime_error("Malformed sphere\nEx: (0 0 0) 1 material_name");
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
    else {
        throw std::runtime_error("Malformed triangle\nEx: (-1 -1 1) (1 -1 2) (0 -1 1) material_name");
    }
}

void Parser::parse_rectangle(std::stringstream& ss) {
    Point3D bottom_left, bottom_right;
    double width;
    std::string material_name;
    if (ss >> bottom_left >> bottom_right >> width >> material_name) {
        const Material* material = get_material(material_name);
        std::unique_ptr<Object> object = std::make_unique<Rectangle>(bottom_left, bottom_right, width, material);
        world.add(std::move(object));
    }
    else {
        throw std::runtime_error("Malformed rectangle\nEx: (0 0 0) (10 0 0) 5 material_name");
    }
}

void Parser::parse_constant_medium(std::stringstream& ss) {
    // only allows for spherical boundaries
    Vector3D center;
    double radius, density;
    std::string material_name;
    if(ss >> center >> radius >> density >> material_name) {
        const Material* material = get_material(material_name);
        auto boundary = new Sphere{center, radius, material};
        std::unique_ptr<Object> object = std::make_unique<ConstantMedium> (boundary, density, material);
        world.add(std::move(object));
    }
    else {
        throw std::runtime_error("Malformed_constant_medium!");
    }
}

void Parser::parse_mesh(std::stringstream& ss) {
    // mesh position filename material_name
    Vector3D position;
    std::string filename, material_name;
    if (!(ss >> position >> filename >> material_name)) {
        throw std::runtime_error("Malformed mesh\nEx: (0 0 0) mesh_file material_name");
    }

    const Material* material = get_material(material_name);

    std::ifstream input{filename};
    if(!input) {
        throw std::runtime_error("Cannot open mesh file: " + filename);
    }

    std::string temp;
    input >> temp;
    if (temp != "vertices") {
        throw std::runtime_error("Mesh file must start with 'vertices'");
    }

    std::vector<Vector3D> vertices;
    for (Vector3D vertex; input >> vertex;) {
        vertices.push_back(vertex + position);
    }
    if (vertices.size() < 3) {
        throw std::runtime_error("Mesh file must contain at least 3 vertices");
    }

    // input attempted to read "triangles-input" > Vector3D
    input.clear(); // clears error bit
    input >> temp; //"triangles-input"
    // if (temp != "triangles-input") {
    //     throw std::runtime_error("Mesh file must contain 'triangles-input'");
    // }
    // read each line under triangles-input
    for (int a, b, c; input >> a >> b >> c;) {
        std::unique_ptr<Object> triangle = std::make_unique<Triangle>(vertices.at(a), vertices.at(b),
                                                                            vertices.at(c), material);
        world.add(std::move(triangle));
    }

}

void Parser::parse_obj(std::stringstream& ss) {
    std::string command;
    ss >> command;

    std::ifstream input{filename};
    if(!input) {
        throw std::runtime_error("Cannot open mesh file: " + filename);
    }

    std::vector<Point3D> vertices;
    if (command == "v") {
        for (Point3D vertex; input >> vertex;) {
            vertices.push_back(vertex);
        }
    }
    if (command == "f") {
        // Access vertices and find point and that number in the vector
        std::vector<Point3D> face;
        // Point3D input;
        for (Point3D vertex; input >> vertex;) {
            face.push_back(vertex);
        }
        if (face.size() < 3) {
            throw std::runtime_error("Face must contain at least 3 vertices");
        }
        if (face.size() == 3) {
            // create a triangle

        }
    }
}

void Parser::parse_camera(std::stringstream& ss) {
    if (ss >> camera_position >> camera_target >> camera_up >> camera_fov) {
        found_camera = true;
    }
    else {
        throw std::runtime_error("Malformed camera\nEx:  (2 -8 4) (1.5 0 1) (0 0 1) 90");
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
