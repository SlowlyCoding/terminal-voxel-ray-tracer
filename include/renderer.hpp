#pragma once
#include "config.hpp"
#include "camera.hpp"
#include "pixelbuffer.hpp"
#include "intersection_information.hpp"
#include "vector.hpp"
#include "object.hpp"
#include "ray.hpp"
#include <vector>
#include <thread>

class Renderer {
    private:
        /* scene */
        PixelBuffer *pixelbuffer;
        Camera *camera;
        Object *object;

        /* settings */
        int thread_amount = std::thread::hardware_concurrency();
        int max_ray_bounces;
        unsigned int ao_samples;

        /* skybox */
        bool skybox_enabled;
        int skybox_width, skybox_height, skybox_channels;
        uint8_t* skybox;

        /* Traces a ray through the scene and and returns the color of that pixel. */
        RGB trace_ray(Ray *ray, int max_ray_bounces);
    public:
        Renderer(Config& config, PixelBuffer *_pixelbuffer, Camera *_camera, Object *_object);
        /* render_framepart() renders a specified part of the frame */
        void render_framepart(Vec3f pixel0, Vec3f pixel_step_x, Vec3f pixel_step_y, int thread_amount, int part);
        /* creates threads and calls render_framepart() */
        void threaded_render();
};
