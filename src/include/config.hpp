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
  Vec3f camera_view_direction;
  Vec3f camera_view_up;
  int camera_fov;

  Vec3f octree_center;
  float octree_side_length;
  int octree_depth;
  bool octree_same_voxel_size;

  Vec3f light_position;

  bool renderer_shadows_enabled;

  Config(std::string config_file_name);
};
