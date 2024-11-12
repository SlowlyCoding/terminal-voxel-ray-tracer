#pragma once
#include "vector.hpp"

struct Ray {
    Vec3f origin;
    Vec3f direction;
    Vec3f inv_direction;
    int sign[3];
    int stepx, stepy, stepz;
    float max_t;
    float min_t;

    Ray(Vec3f _origin, Vec3f _direction) {
        origin = _origin;
        direction = _direction.normalize();
        inv_direction = Vec3f(1.f/direction.x, 1.f/direction.y, 1.f/direction.z);
        sign[0] = (inv_direction.x < 0);
        sign[1] = (inv_direction.y < 0);
        sign[2] = (inv_direction.z < 0);
        direction.x > 0 ? stepx = 1 : stepx = -1;
        direction.y > 0 ? stepy = 1 : stepy = -1;
        direction.z > 0 ? stepz = 1 : stepz = -1;
        max_t = 9999.9;
        min_t = 0.0;
    };
    Vec3f point(const float t) const {return origin + direction*t;};
};
