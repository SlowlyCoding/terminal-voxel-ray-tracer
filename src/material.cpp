#include "../include/material.hpp"

RGB::RGB(std::string color) {
    if (color == "background") {
        r=22; g=22; b=22;
    } else if (color == "black") {
        r=0; g=0; b=0;
    } else if (color == "white") {
        r=255; g=255; b=255;
    } else if (color == "random") {
        r=(int)(rand()%256); g=(int)(rand()%256); b=(int)(rand()%256);
    } else if (color == "orange") {
        r=255; g=140; b=0;
    } else if (color == "purple") {
        r=175; g=35; b=255;
    } else if (color == "green") {
        r=0; g=255; b=0;
    } else {
        r=0; g=0; b=0;
    }
}

RGB RGB::operator+(RGB color) const {
    return RGB(r+color.r, g+color.g, b+color.b);
} 
RGB RGB::operator-(RGB color) const {
    return RGB(r-color.r, g-color.g, b-color.b);
} 
RGB RGB::operator*(int n) const {
    return RGB(r*n, g*n, b*n);
}
RGB RGB::operator*(float n) const {
    return RGB(r*n, g*n, b*n);
}
RGB RGB::operator/(int n) const {
    return RGB(r/n, g/n, b/n);
}
RGB RGB::operator/(float n) const {
    return RGB(r/n, g/n, b/n);
}

void RGB::normalize() {
    if (r>255) {r=255;}
    if (g>255) {g=255;}
    if (b>255) {b=255;}
}

Material::Material(MaterialType _type, RGB _color, float _tint_strength) {
    type = _type;
    color = _color;
    assert(_tint_strength >= 0.0 && _tint_strength <= 1.0);
    tint_strength = _tint_strength;
}

Material new_material_standard(RGB color) {
    return Material(standard, color, 0.0);
}
Material new_material_reflective(RGB tint, float tint_strength) {
    return Material(reflective, tint, tint_strength);
}
