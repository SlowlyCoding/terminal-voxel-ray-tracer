#pragma once
#include "config.hpp"
#include "vector.hpp"

class Camera {
  public:
    //camera position
    Vec3f view_point; 
    // orientation angles
    float view_angle_x;
    float view_angle_z;
    // orientation vector
    Vec3f view_direction;
    Vec3f view_up;
    float FOV;
    Camera(Config *config);
};

