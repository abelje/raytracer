#include "parser.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

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
    }
    catch (std::runtime_error& e) {
      std::cout << "Error when parsing line:\n" << line << '\n';
      throw e;
    }
  }
}