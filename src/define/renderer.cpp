#include "../include/renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../include_external/nothings/stb_image.h"

Renderer::Renderer(Config *config, PixelBuffer *_pixelbuffer, Camera *_camera, Octree *_octree) {
  pixelbuffer = _pixelbuffer;
  camera = _camera;
  octree = _octree;
  lights = config->lights;
  light_intensities = config->light_intensities;
  max_ray_bounces = config->renderer_max_ray_bounces;
  std::cout << "Using " << thread_amount << " threads\n";

  skybox_enabled = config->skybox_enabled;
  if (skybox_enabled) {
    std::cout << "loading in skybox...";
    skybox = stbi_load(config->skybox_file.c_str(), &skybox_width, &skybox_height, &skybox_channels, 3);
    if (skybox == nullptr) {
      skybox_enabled = false;
      std::cout << "failed!\n";
    }
    std::cout << "done!\n";
  }
}

RGBi Renderer::trace_ray(Ray *ray, int max_ray_bounces) {
  RGBi pixel("background");
  intersection_information ii;
  if (octree->intersection(ray, &ii, false)) {
    switch (ii.material->type) {
    case standard: {
      for (int i=0; i<lights.size(); i++) {
        Vec3f l = lights[i] - ii.point;
        float r = l.length();
        l = l.normalize();
        Vec3f h = (ray->direction*-1.0f + l) / (ray->direction*-1.0f + l).length();
        // check if surface is in shade
        Ray light_ray(ii.point, l);
        if (dot(ii.normal,l) > 0) light_ray.origin = ii.point+l*0.01f;
        else light_ray.origin = ii.point-l*0.01f;
        if (octree->intersection(&light_ray, &ii, true)) {
          pixel = pixel + ii.material->color*0.3f;
          continue;
        }
        // calculate light
        float Ld = ii.material->diffuse*(light_intensities[i]/r*r)*std::max(0.0f,dot(ii.normal,l));
        float Ls = ii.material->specular*(light_intensities[i]/r*r)*std::max(0.0f,dot(ii.normal,h));
        pixel = pixel + ii.material->color*Ld + ii.material->color*Ls;
      }
      pixel = pixel / (float)lights.size();
      break;
    }
    case reflective: {
      if (max_ray_bounces > 0) {
        Vec3f reflected_ray_direction = ray->direction - ii.normal * 2.f*dot(ray->direction,ii.normal);
        Ray reflected_ray(ii.point + reflected_ray_direction*0.01f, reflected_ray_direction);
        pixel = ii.material->color*ii.material->tint_strength + 
                trace_ray(&reflected_ray, max_ray_bounces-1) * (1.0f-ii.material->tint_strength);
      } else {
        pixel = RGBi("black");
      }
      break;
    }
    case refractive: {
      if (max_ray_bounces > 0) {
        Vec3f reflected_ray_direction = ray->direction - ii.normal * 2.f*dot(ray->direction,ii.normal);
        Ray reflected_ray(ii.point + reflected_ray_direction*0.01f, reflected_ray_direction);

        float n1 = 1.0; // air
        float n2 = ii.material->refractive_index;
        if (ii.inside_voxel) std::swap(n1, n2);
        float n = n1 / n2;

        float cosI = -dot(ii.normal, ray->direction);
        float sinT2 = n*n*(1.0-cosI*cosI);
        if (sinT2 > 1.0) { // total internal reflection
          pixel = trace_ray(&reflected_ray, max_ray_bounces-1);
          break;
        }
        float cosT = sqrt(1.0 - sinT2);

        float rOrth = (n1*cosI-n2*cosT)/(n1*cosI+n2*cosT);
        float rPar = (n2*cosI -n1*cosT)/(n2*cosI+n1*cosT);
        float reflectance = (rOrth*rOrth+rPar*rPar)/2.0;

        Vec3f refracted_ray_direction = ray->direction*n+ii.normal*(n*cosI-cosT);
        Ray refracted_ray(ii.point + refracted_ray_direction*0.01f, refracted_ray_direction);

        // mix reflection and refraction
        pixel = trace_ray(&reflected_ray, max_ray_bounces-1)*reflectance+trace_ray(&refracted_ray, max_ray_bounces-1)*(1.0f-reflectance);
        // add a color to that mix
        pixel = pixel*(1.0f-ii.material->tint_strength)+ii.material->color*ii.material->tint_strength;
      } else {
        pixel = RGBi("black");
      }
      break;
    }
    case normal: {
      pixel = RGBi((ii.normal.x+1.)*127.5, (ii.normal.y+1.)*127.5, (ii.normal.z+1.)*127.5);
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
      pixel = RGBi(
          static_cast<int>(skybox[int(y*skybox_width*3 + x*3)]), 
          static_cast<int>(skybox[int(y*skybox_width*3 + x*3 + 1)]), 
          static_cast<int>(skybox[int(y*skybox_width*3 + x*3 + 2)]));
    } else {
      pixel = RGBi("background");
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
      RGBi pixel_color = trace_ray(&ray, max_ray_bounces);
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
