#pragma once
#include <string>

class RGBi {
public:
  int r,g,b;
  RGBi() : r(0), g(0), b(0) {};
  RGBi(int r, int g, int b) : r(r), g(g), b(b) {};
  RGBi(std::string color);
  friend RGBi operator + (RGBi color1, RGBi color2);
  friend RGBi operator - (RGBi color1, RGBi color2);
  friend RGBi operator * (RGBi color, int n);
  friend RGBi operator * (RGBi color, float n);
  friend RGBi operator / (RGBi color, int n);
  friend RGBi operator / (RGBi color, float n);
  void normalize();
  void invert();
};

class Material {
public:
  RGBi color;
  float diffuse;
  float specular;
  std::string type = "none";
  float reflection_factor;
  // none
  Material() : color(RGBi("black")), diffuse(0.) {};
  // normal color
  Material(RGBi color, float diffuse, float specular) : 
    color(color), diffuse(diffuse), specular(specular) {};
  // refelctive, refractive
  Material(RGBi color, float diffuse, float specular, std::string type, float factor) : 
    color(color), diffuse(diffuse), specular(specular), type(type) {
    if (type == "reflective") {
      reflection_factor = factor;
    }
  };
};
