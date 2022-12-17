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
    reflectance [ 0-1 ]
      determines how much % of the incoming light gets reflected if MaterialType is reflective
  */
  float reflectance;
  /*
    refractive_index
      air = 1.0
      water = 1.33
      glass = 1.5
  */
  float refractive_index;

  Material() : type(normal) {};
  Material(MaterialType type, RGBi color, float diffuse, float specular, float reflectance, float refractive_index) :
    type(type), color(color), diffuse(diffuse), specular(specular), reflectance(reflectance), refractive_index(refractive_index) {};
};

Material new_material_standard(RGBi color, float diffuse, float specular);
Material new_material_reflective(RGBi color, float diffuse, float specular, float reflectance);
Material new_material_refractive(float refraction_index);
