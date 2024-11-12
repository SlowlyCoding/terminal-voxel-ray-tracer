#include "vector.hpp"

Vec3f Vec3f::operator+(Vec3f const& v) const {
    return Vec3f(x + v.x, y + v.y, z + v.z);
}
Vec3f Vec3f::operator-(Vec3f const& v) const {
    return Vec3f(x - v.x, y - v.y, z - v.z);
}
Vec3f Vec3f::operator*(float s) const {
    return Vec3f(x*s, y*s, z*s);
}
Vec3f Vec3f::operator*(int s) const {
    return Vec3f(x*s, y*s, z*s);
}
Vec3f Vec3f::operator/(float s) const {
    return Vec3f(x/s, y/s, z/s);
}
Vec3f Vec3f::operator/(int s) const {
    return Vec3f(x/s, y/s, z/s);
}

float Vec3f::length() const {
    return sqrt(x*x + y*y + z*z);
}
Vec3f Vec3f::normalize() const {
    float l = length();
    return Vec3f(x/l, y/l, z/l);
}

std::ostream& operator<<(std::ostream& out, Vec3f& v) {
    return out << '(' << v.x << ',' << v.y << ',' << v.z << ')';
}

float dot(Vec3f const& v1, Vec3f const& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
Vec3f cross(Vec3f const& v1, Vec3f const& v2) {
    return Vec3f(
            v1.y*v2.z - v1.z*v2.y,
            v1.z*v2.x - v1.x*v2.z,
            v1.x*v2.y - v1.y*v2.x);
}
