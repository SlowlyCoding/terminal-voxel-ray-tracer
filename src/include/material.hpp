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
};

enum MaterialType {
  standard,
  normal, // rgb of surface color is xyz of surface normal vector
  reflective,
  refractive
};

class Material {
public:
  MaterialType type;
  RGBi color;
  float diffuse;
  float specular;
  /*
    reflection factor [ 0-1 ]
      determines how much % of the incoming light gets reflected if MaterialType is reflective or refractive
  */
  float reflection_factor;
  float refractive_index = 1.5;

  // none
  Material() : type(normal) {};
  // standard or normal
  Material(MaterialType type, RGBi color, float diffuse, float specular) :
    type(type), color(color), diffuse(diffuse), specular(specular) {};
  // reflective or refractive
  Material(MaterialType type, RGBi color, float diffuse, float specular, float reflection_factor) :
    type(type), color(color), diffuse(diffuse), specular(specular), reflection_factor(reflection_factor) {};
};
