#pragma once
#include "config.hpp"
#include "camera.hpp"
#include "pixelbuffer.hpp"
#include "intersection_information.hpp"
#include "vector.hpp"
#include "octree.hpp"
#include "ray.hpp"
#include <vector>
#include <thread>

class Renderer {
  private:
    PixelBuffer *pixelbuffer;
    Camera *camera;
    Octree *octree;
    Vec3f light;
    bool shadows_enabled;

    bool skybox_enabled;
    int skybox_width, skybox_height, skybox_channels;
    unsigned char *skybox;
  public:
    Renderer(Config *config, PixelBuffer *pixelbuffer, Camera *_camera, Octree *_octree);
    /* Traces a ray through the scene and and returns the "color" of that pixel. */
    /* In this ray tracer colors are displayed using characters */
    RGB trace_ray(Ray *ray);
    /* Renders the Scene by calcuating what character each pixel should display. */
    /* render_framepart() renders a specified part of the frame */
    void render_framepart(Vec3f pixel0, Vec3f pixel_step_x, Vec3f pixel_step_y, int thread_amount, int part);
    /* creates threads and calls render_framepart() */int thread_amount = std::thread::hardware_concurrency();
    void threaded_render();
};
