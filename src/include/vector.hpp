#pragma once
#include <iostream>
#include <math.h>

class Vec3f {
public:
  float x,y,z;
  Vec3f() : x(-1.), y(-1.), z(-1.) {};
  Vec3f(float x, float y, float z) : x(x), y(y), z(z) {};
  Vec3f friend operator + (Vec3f v1, Vec3f v2);
  Vec3f friend operator - (Vec3f v1, Vec3f v2);
  Vec3f friend operator * (Vec3f v, float s);
  Vec3f friend operator * (Vec3f v, int s);
  Vec3f friend operator / (Vec3f v, float s);
  Vec3f friend operator / (Vec3f v, int s);
  float length();
  Vec3f normalize();
  void values();
};
float dot(Vec3f v1, Vec3f v2);
Vec3f cross(Vec3f v1, Vec3f v2);
