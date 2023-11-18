#include "../include/renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../include_external/nothings/stb_image.h"

Renderer::Renderer(Config& config, PixelBuffer *_pixelbuffer, Camera *_camera, Object *_object) {
    pixelbuffer = _pixelbuffer;
    camera = _camera;
    object = _object;
    max_ray_bounces = config.renderer_max_ray_bounces;
    ao_samples = config.renderer_ao_samples;
    std::cout << "Using " << thread_amount << " threads\n";

    skybox_enabled = config.skybox_enabled;
    if (skybox_enabled) {
        std::cout << "loading in skybox..." << std::endl;
        skybox = stbi_load(config.skybox_file.c_str(), &skybox_width, &skybox_height, &skybox_channels, 3);
        if (skybox == nullptr) {
            skybox_enabled = false;
        }
    }
}

RGB Renderer::trace_ray(Ray *ray, int max_ray_bounces) {
    RGB pixel("background");
    intersection_information ii;
    if (object->intersect(*ray, &ii, false)) {
        switch (ii.material->type) {
            case normal:{
                pixel.r = (ii.normal.x+1)*128;
                pixel.g = (ii.normal.y+1)*128;
                pixel.b = (ii.normal.z+1)*128;
                break;
            }
            case standard: {
                // calculate ambient occlusion by shooting new rays from hit point
                // and checking if the intersect anything
                intersection_information _;
                int occlusion = 0;
                for (int i=0; i<ao_samples; i++) {
                    Vec3f direction = ii.normal;
                    if (ii.normal.x == 0) direction.x = ((double)rand()/RAND_MAX-0.5)*5.6f;
                    if (ii.normal.y == 0) direction.y = ((double)rand()/RAND_MAX-0.5)*5.6f;
                    if (ii.normal.z == 0) direction.z = ((double)rand()/RAND_MAX-0.5)*5.6f;
                    direction = direction;
                    Ray occlusion_ray(ii.point+ii.normal*0.001f, direction);
                    if (object->intersect(occlusion_ray, &_, false)) occlusion++;
                }
                pixel = ii.material->color * (1 - 0.5f*occlusion/ao_samples);
                break;
            }
            case reflective: {
                if (max_ray_bounces > 0) {
                    Vec3f reflected_ray_direction = ray->direction - ii.normal * 2.f*dot(ray->direction,ii.normal);
                    Ray reflected_ray(ii.point + reflected_ray_direction*0.01f, reflected_ray_direction);
                    pixel = ii.material->color*ii.material->tint_strength + 
                        trace_ray(&reflected_ray, max_ray_bounces-1) * (1.0f-ii.material->tint_strength);
                } else {
                    pixel = RGB("black");
                }
                break;
            }
        }
    } else { // ray did not hit any objects
        if (skybox_enabled) {
            Vec3f p = ray->point(ray->max_t);
            Vec3f d = (Vec3f(0.,0.,0.) - p).normalize();
            float u = 0.5 + atan2(d.x, d.y) / (2. * 3.141592);
            float v = 0.5 + asin(d.z) / 3.141592;
            int x = u*skybox_width;
            int y = v*skybox_height;
            pixel = RGB(
                    static_cast<int>(skybox[int(y*skybox_width*3 + x*3)]), 
                    static_cast<int>(skybox[int(y*skybox_width*3 + x*3 + 1)]), 
                    static_cast<int>(skybox[int(y*skybox_width*3 + x*3 + 2)]));
        } else {
            pixel = RGB("background");
        }
    }
    return pixel;
}

void Renderer::render_framepart(
        Vec3f pixel0, Vec3f pixel_step_x, Vec3f pixel_step_y, int thread_amount, int part) {
    // assign each thread a part of the frame
    int y = pixelbuffer->height * (1./thread_amount) * part;
    int y_max = pixelbuffer->height * (1./thread_amount) * (part+1);
    // go through each pixel of that part and call trace_ray()
    for (; y<y_max; y++) {
        for (int x=0; x<pixelbuffer->width; x++) {
            Vec3f pixel = pixel0 + pixel_step_x*x + pixel_step_y*y;
            Ray ray(camera->view_point, pixel.normalize());
            RGB pixel_color = trace_ray(&ray, max_ray_bounces);
            pixel_color.normalize();
            pixelbuffer->pixels[y][x][0] = pixel_color.r;
            pixelbuffer->pixels[y][x][1] = pixel_color.g;
            pixelbuffer->pixels[y][x][2] = pixel_color.b;
        }
    }
}
void Renderer::threaded_render() {
    // calculating different camera vectors
    Vec3f half_screen_x = cross(camera->view_direction, camera->view_up); 
    Vec3f half_screen_y = cross(camera->view_direction, half_screen_x);
    half_screen_x = half_screen_x * (float)tan((camera->FOV/2.)*3.141592/180.);
    half_screen_y = half_screen_y * (float)tan(((camera->FOV*((float)pixelbuffer->height/pixelbuffer->width))/2.)*3.141592/180.)*1.f;
    Vec3f pixel0 = camera->view_direction - half_screen_x - half_screen_y;
    Vec3f pixel_step_x = half_screen_x / ((float)pixelbuffer->width/2);
    Vec3f pixel_step_y = half_screen_y / ((float)pixelbuffer->height/2);

    // each thread renders its own part of the frame
    std::vector<std::thread> threads;
    for (int i=0; i<thread_amount; i++) {
        threads.push_back(std::thread(&Renderer::render_framepart, this, pixel0, pixel_step_x, pixel_step_y, thread_amount, i));
    }
    for (auto& t : threads) t.join();
}
