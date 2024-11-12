#pragma once
#include "vector.hpp"
#include <string>
#include <nlohmann/json.hpp>

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
        float camera_speed;
        float camera_mouse_sensitivity;
        bool camera_invert_mouse;

        std::string object_model_path;
        Vec3f object_location;
        float object_voxel_size;
        uint8_t object_color_index;

        int renderer_max_ray_bounces;
        std::string renderer_grayscale;
        unsigned int renderer_ao_samples;

        bool skybox_enabled;
        std::string skybox_file;

        Config(std::string config_file_name);
};
