#include "parser.h"
#include "color.h"
#include "diffuse.h"
#include "metallic.h"
#include "camera.h"
#include "pixels.h"
#include "sphere.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "glass.h"
#include "lambertian.h"
#include "specular.h"

Parser::Parser(const std::string& filename)
    :filename{filename} {
  std::ifstream input{filename};
  if (!input) {
    std::string msg{"Could not open filename: " + filename};
    throw std::runtime_error{msg};
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

void Parser::verify() {}

void remove_comment(std::string& line) {
  auto i = line.find('#');
  if (i != std::string::npos) {// found a #
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
      else if (type == "output") {
        parse_output(ss);
      }
      else if (type == "pixels") {
        parse_pixels(ss);
      }
      else if (type == "rays") {
        parse_rays(ss);
      }
      else {
        throw std::runtime_error{"Unknown type: " + type + " in line " + line};
      }
    }
    catch (std::runtime_error& e) {
      std::cout << "Error when parsing line:\n" << line << '\n';
      throw e;
    }
  }
}

void Parser::parse_material(std::stringstream& ss) {
  std::string name, kind;
  Color color;
  bool emitting;

  if (!(ss >> name >> kind >> color >> std::boolalpha >> emitting)) {
    throw std::runtime_error("Malformed material");
    // Maybe add line counter?
  }

  // add materials into here
  if (kind == "diffuse") {
    materials[name] = std::make_unique<Diffuse>(color, emitting);
  }
  else if (kind == "metallic") { //needs fuzz value
    double fuzz;
    if (ss >> fuzz) { // fuzz exists
      materials[name] = std::make_unique<Metallic>(color, emitting, fuzz);
    }
    else {
      throw std::runtime_error("Missing parameter 'fuzz' for material: " + name);
    }
  }

  else if (kind == "specular") {
    materials[name] = std::make_unique<Specular>(color, emitting);
  }

  else if (kind == "lambertian") {
    materials[name] = std::make_unique<Lambertian>(color, emitting);
  }

  else if (kind == "glass") {
    materials[name] = std::make_unique<Glass>(color, emitting);
  }

  else {
    throw std::runtime_error("Unknown material kind: '" + kind + "'");
  }
}

Material* Parser::get_material(const std::string& material) {
  auto i = materials.find(material);
  if (i != materials.end()) {
    return i->second.get();
  }
  else {
    throw std::runtime_error("Unknown material: " + material);
  }
}

void Parser::parse_sphere(std::stringstream& ss) {
  Vector3D center;
  double radius;
  std::string material_name;

  if(ss >> center >> radius >> material_name) {
    const Material* material = get_material(material_name);
    world.add(center, radius, material);
  }
  else {
    throw std::runtime_error("Malformed sphere");
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
    	// found_rays
	}
}
void Parser::parse_pixels(std::stringstream& ss) {
	if (ss >> columns >> rows) {
    	aspect = static_cast<double>(columns) / rows;
        found_pixels = true;
	}
    else {
        throw std::runtime_error("Malformed pixels");
    }
}
void Parser::parse_output(std::stringstream& ss) {
	if (ss >> filename) {
    	// found_output
	}
}