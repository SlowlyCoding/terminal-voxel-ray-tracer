#pragma once
#include "vector.hpp"
#include "material.hpp"

struct intersection_information {
  float t;
  Vec3f point; 
  Vec3f normal;
  Material material;
};
