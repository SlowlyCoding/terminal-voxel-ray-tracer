#include "../include/renderer.hpp"

Renderer::Renderer(
    int _window_width, int _window_height, int *_pixels,
    Camera *_camera, Octree *_octree, Vec3f *_light, bool _shadows_enabled) {
  window_width = _window_width;
  window_height = _window_height;
  shadows_enabled = _shadows_enabled;
  pixels = _pixels;
  camera = _camera;
  octree = _octree;
  light = _light;
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
      pixel = ii.material.color + trace_ray(&reflected_ray) * ii.material.reflection_factor;
    } else {
      // if the object is not refelctive we do shading
      Vec3f l = (*light - ii.point).normalize();
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
  }
  return pixel;
}

void Renderer::render_framepart(
    Vec3f pixel0, Vec3f pixel_step_x, Vec3f pixel_step_y, int thread_amount, int part) {
  // assign each thread a part of the frame
  int y = window_height * (1./thread_amount) * part;
  int y_max = window_height * (1./thread_amount) * (part+1);
  // go through each pixel of that part and call trace_ray()
  for (y; y<y_max; y++) {
    for (int x=0; x<window_width; x++) {
      Vec3f pixel = pixel0 + pixel_step_x*x + pixel_step_y*y;
      Ray ray(camera->view_point, pixel.normalize());
      RGB pixel_color = trace_ray(&ray);
      pixel_color.normalize();
      pixels[y*window_width*3 + x*3] = pixel_color.r;
      pixels[y*window_width*3 + x*3 + 1] = pixel_color.g;
      pixels[y*window_width*3 + x*3 + 2] = pixel_color.b;
    }
  }
}
void Renderer::threaded_render() {
  // calculating different camera vectors
  Vec3f half_screen_x = cross(camera->view_direction, camera->view_up); 
  Vec3f half_screen_y = cross(camera->view_direction, half_screen_x);
  half_screen_x = half_screen_x * (float)tan((camera->FOV/2.)*3.141592/180.);
  half_screen_y = half_screen_y * (float)tan(((camera->FOV*((float)window_height/window_width))/2.)*3.141592/180.)*2.f;
  Vec3f pixel0 = camera->view_direction - half_screen_x - half_screen_y;
  Vec3f pixel_step_x = half_screen_x / ((float)window_width/2);
  Vec3f pixel_step_y = half_screen_y / ((float)window_height/2);

  // each thread renders its own part of the frame
  int thread_amount = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  for (int i=0; i<thread_amount; i++) {
    threads.push_back(std::thread(&Renderer::render_framepart, this, pixel0, pixel_step_x, pixel_step_y, thread_amount, i));
  }
  for (auto& t : threads) t.join();
}

void Renderer::create_ppm() {
  FILE *image_file;
  image_file = fopen("image.ppm", "wb");
  fprintf(image_file, "P6\n");
  fprintf(image_file, "%d %d\n", window_width, window_height);
  fprintf(image_file, "255\n");
  fwrite(pixels, 1, window_width * window_height * 3, image_file);
  fclose(image_file);
}
