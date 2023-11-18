#pragma once
#include "config.hpp"
#include "vector.hpp"

class Camera {
    public:
        // orientation vector
        Vec3f view_direction;
        Vec3f view_up;
        Vec3f view_left;
        //camera position
        Vec3f view_point; 
        // orientation angles
        float view_angle_x; // pitch
        float view_angle_z; // yaw
        float FOV;
        Camera(Config& config);
        void view_angle_changed();
};

