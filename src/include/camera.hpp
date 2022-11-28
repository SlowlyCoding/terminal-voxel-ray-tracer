#pragma once
#include "config.hpp"
#include "vector.hpp"

class Camera {
  public:
    // location of camera
    Vec3f view_point; 
    // in which direction the camera is looking
    Vec3f view_direction;
    // view_direction turned 90° so that its showing up, used for calculations in Renderer::threaded_render()
    Vec3f view_up;
    float FOV;
    Camera(Vec3f view_point, Vec3f view_direction, Vec3f view_up, int FOV) : 
      view_point(view_point), view_direction(view_direction.normalize()), view_up(view_up.normalize()), FOV(FOV) {};
    Camera(Config *config);
};

