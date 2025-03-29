#pragma once

#include "texture.h"
#include "color.h"

class Surface_Normal : public Texture {
public:
    Surface_Normal(bool smooth = true);
    Color value(double u, double v) const override;
private:
    bool smooth;
};