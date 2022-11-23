#pragma once
#include <string>

class RGB {
public:
  int r,g,b;
  RGB() : r(0), g(0), b(0) {};
  RGB(int r, int g, int b) : r(r), g(g), b(b) {};
  RGB(std::string color);
  friend RGB operator + (RGB color1, RGB color2);
  friend RGB operator - (RGB color1, RGB color2);
  friend RGB operator * (RGB color, int n);
  friend RGB operator * (RGB color, float n);
  friend RGB operator / (RGB color, int n);
  friend RGB operator / (RGB color, float n);
  void normalize();
  void invert();
};

class Material {
public:
  RGB color;
  float diffuse;
  float specular;
  std::string type = "none";
  float reflection_factor;
  // none
  Material() : color(RGB("black")), diffuse(0.) {};
  // normal color
  Material(RGB color, float diffuse, float specular) : 
    color(color), diffuse(diffuse), specular(specular) {};
  // refelctive, refractive
  Material(RGB color, float diffuse, float specular, std::string type, float factor) : 
    color(color), diffuse(diffuse), specular(specular), type(type) {
    if (type == "reflective") {
      reflection_factor = factor;
    }
  };
};
