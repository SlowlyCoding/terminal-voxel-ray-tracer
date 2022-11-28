#include "../include/camera.hpp"

Camera::Camera(Config *config) {
  view_point = config->camera_view_point;
  view_direction = config->camera_view_direction;
  view_up = config->camera_view_up;
  FOV = config->camera_fov;
}
