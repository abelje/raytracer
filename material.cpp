#include "material.h"

Material::Material(std::string name, Color color, bool emitting)
    :name(name), color(color), emitting(emitting) {}