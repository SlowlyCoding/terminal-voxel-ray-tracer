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

  std::vector<json> light_data = data["lighting"]["pointlights"];
  for (int i=0; i<light_data.size(); i++) {
    lights.push_back(Vec3f(light_data[i][0], light_data[i][1], light_data[i][2]));
    light_intensities.push_back(light_data[i][3]);
  }

  renderer_max_ray_bounces = data["renderer"]["max_ray_bounces"];
  renderer_grayscale = data["renderer"]["grayscale"];

  skybox_enabled = data["skybox"]["enabled"];
  skybox_file = data["skybox"]["file"];
}
