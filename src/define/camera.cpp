#include "../include/camera.hpp"

Camera::Camera(Config *config) {
  view_point = config->camera_view_point;
  view_angle_x = config->camera_view_angle_x;
  view_angle_z = config->camera_view_angle_z;
  view_direction = Vec3f(sin(view_angle_z), cos(view_angle_z), sin(view_angle_x));
  view_up = Vec3f(sin(view_angle_z), cos(view_angle_z), cos(view_angle_x));
  FOV = config->camera_fov;
}
