#include <iostream>
#include "include/config.hpp"
#include "include/terminal.hpp"
#include "include/clock.hpp"
#include "include/pixelbuffer.hpp"
#include "include/camera.hpp"
#include "include/renderer.hpp"
#include "include/octree.hpp"
#include "include/vector.hpp"
#define PI 3.14159265

/*

  TODO:
    refractive objects dont cause shadows -> fix
    improve display_mode 2
    create 2 buffers and only display the pixels which have changed (less set_color calls)
    or use double buffering so that render() doesn't have to wait until display() is finished
    maybe create scene class and store camera, objects, ... in there

*/

int main() {
  /* parse config file */
  Config config("config.json");

  /* init everything and create octree */
  PixelBuffer pixelbuffer(&config);
  Camera camera(&config);

  Material glass = new_material_refractive(1.5);
  Material mirror = new_material_reflective(RGBi("white"), 0.9, 0.2, 0.95);
  Material orange = new_material_standard(RGBi("orange"), 0.9, 0.2);
  Octree root(&config);
  root.fill("cylinder", 5, &glass, false);
  sleep(1);
  root.fill("cylinder", 15, &orange, false);
  /* root.insert_vertex(Vertex(Vec3f(0.45,0.45,0.45), &glass), false); */

  Renderer renderer(&config, &pixelbuffer, &camera, &root);
  Clock clock(&config);
  Terminal terminal(&config);
  show_cursor(false);

  /* main loop */
  float cam_angle = 0; // rad
  while (cam_angle < 4.25*PI) {
    clock.start();
    renderer.threaded_render();
    clock.finished_render();
    terminal.display(&pixelbuffer);
    clock.finished_display();

    // maybe change this here to something like scene.update()
    camera.view_point.x = sin(cam_angle)*1.5+0.5;
    camera.view_point.y = cos(cam_angle)*1.5+0.5;
    camera.view_point.z = sin(cam_angle)+0.5;
    camera.view_angle_x = cos(cam_angle+PI/2)*0.6;
    camera.view_angle_z += 0.3*clock.frametime;
    camera.view_angle_changed();
    cam_angle += 0.3*clock.frametime;

    clock.finished_frame();
    clock.display_performance();
  }
  show_cursor(true);
  return 0;
}
