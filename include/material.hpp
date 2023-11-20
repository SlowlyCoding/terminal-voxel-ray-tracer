#pragma once
#include <string>
#include <cassert>

struct RGB {
    unsigned int r,g,b;
    RGB(unsigned int r=0, unsigned int g=0, unsigned int b=0) : r(r), g(g), b(b) {};
    RGB(std::string color);
    RGB operator+(RGB color) const;
    RGB operator-(RGB color) const;
    RGB operator*(int n) const;
    RGB operator*(float n) const;
    RGB operator/(int n) const;
    RGB operator/(float n) const;
    void normalize();
};

enum MaterialType {
    standard,
    normal, // rgb of surface color is xyz of surface normal vector
    reflective,
};

struct Material {
    MaterialType type;
    RGB color;
    // determines how much % of the incoming light gets reflected
    float tint_strength; // [0-1]

    Material() : type(normal) {};
    Material(MaterialType _type, RGB _color, float _tint_strength);
};

Material new_material_standard(RGB color);
Material new_material_reflective(RGB tint, float tint_strength);
