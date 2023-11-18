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

    camera_view_point = Vec3f(
            data["camera"]["view_point"][0], 
            data["camera"]["view_point"][1], 
            data["camera"]["view_point"][2]);
    camera_view_angle_x = data["camera"]["view_angle_x"];
    camera_view_angle_z = data["camera"]["view_angle_z"];
    camera_fov = data["camera"]["fov"];
    camera_speed = data["camera"]["speed"];
    camera_mouse_sensitivity = data["camera"]["mouse_sensitivity"];
    camera_invert_mouse = data["camera"]["invert_mouse"];

    object_model_path = data["object"]["model_path"];
    object_location = Vec3f(
            data["object"]["location"][0],
            data["object"]["location"][1],
            data["object"]["location"][2]);
    object_voxel_size = data["object"]["voxel_size"];
    object_color_index = data["object"]["color_index"];

    renderer_max_ray_bounces = data["renderer"]["max_ray_bounces"];
    renderer_grayscale = data["renderer"]["grayscale"];
    renderer_ao_samples = data["renderer"]["ambient_occlusion_samples"];

    skybox_enabled = data["skybox"]["enabled"];
    skybox_file = data["skybox"]["file"];
}
