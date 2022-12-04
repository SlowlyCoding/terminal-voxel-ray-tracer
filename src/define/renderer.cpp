#include "../include/renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/nothings/stb_image.h"

Renderer::Renderer(Config *config, PixelBuffer *_pixelbuffer, Camera *_camera, Octree *_octree) {
  pixelbuffer = _pixelbuffer;
  camera = _camera;
  octree = _octree;
  light = config->light_position;
  shadows_enabled = config->renderer_shadows_enabled;

  skybox_enabled = config->renderer_skybox_enabled;
  if (skybox_enabled) {
    skybox = stbi_load(config->renderer_skybox.c_str(), &skybox_width, &skybox_height, &skybox_channels, 3);
    if (skybox == nullptr) {
      skybox_enabled = false;
    }
  }
}

RGB Renderer::trace_ray(Ray *ray) {
  RGB pixel("background");
  intersection_information ii;
  if (octree->intersection(ray, &ii)) {
    if (ii.material.type == "normal") {
      pixel = RGB((ii.normal.x+1.)*127.5, (ii.normal.y+1.)*127.5, (ii.normal.z+1.)*127.5);
    } else if (ii.material.type == "reflective") {
      // if the object that we just hit is reflective we shoot a new ray from that intersection point
      Vec3f reflected_ray_direction = ray->direction - ii.normal * 2.f*dot(ray->direction,ii.normal);
      Ray reflected_ray(ii.point + reflected_ray_direction*0.11f, reflected_ray_direction);
      pixel = ii.material.color*(1.f-ii.material.reflection_factor) + trace_ray(&reflected_ray) * ii.material.reflection_factor;
    } else {
      // if the object is not refelctive we do shading
      Vec3f l = (light - ii.point).normalize();
      RGB diffuse = ii.material.color * ii.material.diffuse * std::max(0.f,dot(ii.normal, l));
      Vec3f bisector = l + ray->direction*-1;
      RGB specular = ii.material.color * ii.material.specular * std::max(0.f,dot(ii.normal, bisector));
      pixel = pixel + diffuse + specular;
      if (shadows_enabled) {
        // if the ray going to the light hits anything, the pixel is in shade
        Ray light_ray(ii.point+l*0.005f, l);
        if (octree->intersection(&light_ray, &ii)) {
          pixel = pixel * 0.2f;
        } 
      } 
    }
  } else {
    if (skybox_enabled) {
      Vec3f p = ray->point(ray->max_t);
      Vec3f d = (Vec3f(0.,0.,0.) - p).normalize();
      float u = 0.5 + atan2(d.x, d.y) / (2. * 3.141592);
      float v = 0.5 + asin(d.z) / 3.141592;
      int x = u*skybox_width;
      int y = v*skybox_height;
      pixel = RGB(static_cast<int>(skybox[int(y*skybox_width*3 + x*3)]), 
                  static_cast<int>(skybox[int(y*skybox_width*3 + x*3 + 1)]), 
                  static_cast<int>(skybox[int(y*skybox_width*3 + x*3 + 2)]));
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
  for (y; y<y_max; y++) {
    for (int x=0; x<pixelbuffer->width; x++) {
      Vec3f pixel = pixel0 + pixel_step_x*x + pixel_step_y*y;
      Ray ray(camera->view_point, pixel.normalize());
      RGB pixel_color = trace_ray(&ray);
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
  int thread_amount = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  for (int i=0; i<thread_amount; i++) {
    threads.push_back(std::thread(&Renderer::render_framepart, this, pixel0, pixel_step_x, pixel_step_y, thread_amount, i));
  }
  for (auto& t : threads) t.join();
}
