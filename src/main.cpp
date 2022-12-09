#include <iostream>
#include "include/config.hpp"
#include "include/terminal.hpp"
#include "include/clock.hpp"
#include "include/pixelbuffer.hpp"
#include "include/camera.hpp"
#include "include/renderer.hpp"
#include "include/octree.hpp"
#include "include/vector.hpp"

/*

  TODO:
    fix display bug
    create 2 buffers and only display the pixels which have changed (less set_color calls)
    maybe create scene class and store camera, objects, ... in there
    fix octree.fill() so the values arent hardcoded

*/

int main() {
  // parse config file
  Config config("config.json");

  // init everything
  PixelBuffer pixelbuffer(&config);
  Camera camera(&config);
  Octree root(&config);
  root.fill("sphere", 100, false);
  Renderer renderer(&config, &pixelbuffer, &camera, &root);
  Clock clock(&config);
  Terminal terminal(&config);
  terminal.show_cursor(false);

  // main loop
  float cam_angle = 0; // rad
  while (cam_angle < 4.25*3.14) {
    clock.start();
    renderer.threaded_render();
    clock.finished_render();
    terminal.display(&pixelbuffer);
    clock.finished_display();

    // maybe change this here to something like scene.update()
    camera.view_point.x = sin(cam_angle)*1.5+0.5;
    camera.view_point.y = cos(cam_angle)*1.5+0.5;
    camera.view_direction = (config.octree_center - camera.view_point).normalize();
    cam_angle += 0.5*clock.frametime;

    clock.finished_frame();
    clock.display_performance();
  }
  terminal.show_cursor(true);
  return 0;
}
