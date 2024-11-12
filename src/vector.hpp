#pragma once
#include <iostream>
#include <math.h>

struct Vec3f {
    float x,y,z;
    Vec3f(float x=-1, float y=-1, float z=-1) : x(x), y(y), z(z) {};

    Vec3f operator+(Vec3f const& v) const;
    Vec3f operator-(Vec3f const& v) const;
    Vec3f operator*(float s) const;
    Vec3f operator*(int s) const;
    Vec3f operator/(float s) const;
    Vec3f operator/(int s) const;

    float length() const;
    Vec3f normalize() const;
};
std::ostream& operator<<(std::ostream& out, Vec3f& v);
float dot(Vec3f const& v1, Vec3f const& v2);
Vec3f cross(Vec3f const& v1, Vec3f const& v2);
