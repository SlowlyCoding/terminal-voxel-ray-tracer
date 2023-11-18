#pragma once
#include <iostream>
#include <math.h>

class Vec3f {
    public:
        float x,y,z;
        Vec3f(float x=-1, float y=-1, float z=-1) : x(x), y(y), z(z) {};
        Vec3f friend operator + (Vec3f const& v1, Vec3f const& v2);
        Vec3f friend operator - (Vec3f const& v1, Vec3f const& v2);
        Vec3f friend operator * (Vec3f const& v, float s);
        Vec3f friend operator * (Vec3f const& v, int s);
        Vec3f friend operator / (Vec3f const& v, float s);
        Vec3f friend operator / (Vec3f const& v, int s);
        float length();
        Vec3f normalize();
        void values();
};
float dot(Vec3f const& v1, Vec3f const& v2);
Vec3f cross(Vec3f const& v1, Vec3f const& v2);
