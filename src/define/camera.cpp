#include "../include/camera.hpp"

Camera::Camera(Config *config) {
  view_point = config->camera_view_point;
  view_angle_x = config->camera_view_angle_x;
  view_angle_z = config->camera_view_angle_z;
  view_direction = Vec3f(sin(view_angle_z)*cos(view_angle_x), 
                         cos(view_angle_z)*cos(view_angle_x), 
                         sin(view_angle_x));
  view_up = Vec3f(sin(view_angle_z)*cos(view_angle_x+1.571), 
                  cos(view_angle_z)*cos(view_angle_x+1.571),
                  sin(view_angle_x+1.571));
  FOV = config->camera_fov;
}

void Camera::view_angle_changed() {
  view_direction.x = sin(view_angle_z)*cos(view_angle_x);
  view_direction.y = cos(view_angle_z)*cos(view_angle_x); 
  view_direction.z = sin(view_angle_x);
  view_up.x = sin(view_angle_z)*cos(view_angle_x+1.571), 
  view_up.y = cos(view_angle_z)*cos(view_angle_x+1.571),
  view_up.z = sin(view_angle_x+1.571);
  view_left = cross(view_up, view_direction);
}
