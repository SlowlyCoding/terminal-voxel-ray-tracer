#pragma once
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

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

  Vec3f light_position;

  std::string renderer_grayscale;
  bool renderer_shadows_enabled;
  bool renderer_skybox_enabled;
  std::string renderer_skybox;

  Config(std::string config_file_name);
};
