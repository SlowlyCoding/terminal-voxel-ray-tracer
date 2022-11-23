#include "../include/vector.hpp"

Vec3f operator + (Vec3f v1, Vec3f v2) {
  return Vec3f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
Vec3f operator - (Vec3f v1, Vec3f v2) {
  return Vec3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
Vec3f operator * (Vec3f v, float s) {
  return Vec3f(v.x * s, v.y * s, v.z * s);
}
Vec3f operator * (Vec3f v, int s) {
  return Vec3f(v.x * s, v.y * s, v.z * s);
}
Vec3f operator / (Vec3f v, float s) {
  return Vec3f(v.x / s, v.y / s, v.z / s);
}
Vec3f operator / (Vec3f v, int s) {
  return Vec3f(v.x / s, v.y / s, v.z / s);
}

float Vec3f::length() {
  return sqrt(x*x + y*y + z*z);
}
Vec3f Vec3f::normalize() {
  float l = length();
  return Vec3f(x/l, y/l, z/l);
}
void Vec3f::values() {
  std::cout << "(" << x << "," << y << "," << z << ")" << std::endl;
}

float dot(Vec3f v1, Vec3f v2) {
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
Vec3f cross(Vec3f v1, Vec3f v2) {
  return Vec3f(v1.y*v2.z - v1.z*v2.y,
               v1.z*v2.x - v1.x*v2.z,
               v1.x*v2.y - v1.y*v2.x);
}
