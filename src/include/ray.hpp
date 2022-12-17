#pragma once
#include "vector.hpp"

struct Ray {
  Vec3f origin;
  Vec3f direction;
  float max_t = 9999.9;
  float min_t = 0.0;
  Ray(Vec3f origin, Vec3f direction) : origin(origin), direction(direction) {};
  Vec3f point(float t) {return origin + direction*t;};
};
