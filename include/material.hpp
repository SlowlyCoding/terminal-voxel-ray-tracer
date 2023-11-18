#pragma once
#include <string>

class RGB {
    public:
        int r,g,b;
        RGB(int r=0, int g=0, int b=0) : r(r), g(g), b(b) {};
        RGB(std::string color);
        friend RGB operator + (RGB color1, RGB color2);
        friend RGB operator - (RGB color1, RGB color2);
        friend RGB operator * (RGB color, int n);
        friend RGB operator * (RGB color, float n);
        friend RGB operator / (RGB color, int n);
        friend RGB operator / (RGB color, float n);
        void normalize();
        bool operator==(RGB other);
};

enum MaterialType {
    standard,
    normal, // rgb of surface color is xyz of surface normal vector
    reflective,
};

class Material {
    public:
        MaterialType type;
        RGB color;
        float diffuse;
        float specular;
        // determines how much % of the incoming light gets reflected
        float tint_strength; // [0-1]

        Material() : type(normal) {};
        Material(MaterialType type, RGB color, float diffuse, float specular, float tint_strength, float refractive_index) :
            type(type), color(color), diffuse(diffuse), specular(specular), tint_strength(tint_strength) {};

        bool operator==(Material other);
};

Material new_material_standard(RGB color, float diffuse, float specular);
Material new_material_reflective(RGB tint, float tint_strength);
