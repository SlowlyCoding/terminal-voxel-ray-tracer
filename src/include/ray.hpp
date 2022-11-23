#pragma once
#include "vector.hpp"

struct Ray {
  Vec3f origin;
  Vec3f direction;
  Ray(Vec3f origin, Vec3f direction) : origin(origin), direction(direction) {};
  float max_t = 9999.;
  float min_t = 0.;
  Vec3f point(float t) {return origin + direction*t;};
};
