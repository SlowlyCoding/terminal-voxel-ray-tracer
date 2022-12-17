#include "../include/config.hpp"

Config::Config(std::string config_file_name) {
  std::ifstream f(config_file_name);
  json data = json::parse(f);

  terminal_fullscreen = data["terminal"]["fullscreen"];
  terminal_width = data["terminal"]["width"];
  terminal_height = data["terminal"]["height"];
  terminal_display_mode = data["terminal"]["display_mode"];
  terminal_fps_limit_enabled = data["terminal"]["fps_limit_enabled"];
  terminal_fps_limit = data["terminal"]["fps_limit"];

  camera_view_point = Vec3f(data["camera"]["view_point"][0], 
                            data["camera"]["view_point"][1], 
                            data["camera"]["view_point"][2]);
  camera_view_angle_x = data["camera"]["view_angle_x"];
  camera_view_angle_z = data["camera"]["view_angle_z"];
  camera_fov = data["camera"]["fov"];

  octree_center = Vec3f(data["octree"]["center"][0], 
                                 data["octree"]["center"][1], 
                                 data["octree"]["center"][2]);
  octree_side_length = data["octree"]["side_length"];
  octree_depth = data["octree"]["depth"];
  octree_same_voxel_size = data["octree"]["same_voxel_size"];

  light_position = Vec3f(data["light"]["position"][0], 
                         data["light"]["position"][1], 
                         data["light"]["position"][2]);

  renderer_max_ray_bounces = data["renderer"]["max_ray_bounces"];
  renderer_grayscale = data["renderer"]["grayscale"];
  renderer_shadows_enabled = data["renderer"]["shadows_enabled"];
  renderer_skybox_enabled = data["renderer"]["skybox_enabled"];
  renderer_skybox = data["renderer"]["skybox"];
}
