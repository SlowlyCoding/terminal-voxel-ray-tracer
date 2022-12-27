#pragma once
#include <string>
#include <fstream>
#include "../include_external/nlohmann/json.hpp"

#include "vector.hpp"

using json = nlohmann::json;

class Config {
public:
  bool terminal_fullscreen;
  int terminal_width;
  int terminal_height;
  int terminal_display_mode;
  int terminal_fps_limit_enabled;
  int terminal_fps_limit;

  Vec3f camera_view_point;
  float camera_view_angle_x;
  float camera_view_angle_z;
  int camera_fov;

  Vec3f octree_center;
  float octree_side_length;
  int octree_depth;
  bool octree_same_voxel_size;

  std::vector<Vec3f> lights;
  std::vector<float> light_intensities;

  int renderer_max_ray_bounces;
  std::string renderer_grayscale;

  bool skybox_enabled;
  std::string skybox_file;

  Config(std::string config_file_name);
};
