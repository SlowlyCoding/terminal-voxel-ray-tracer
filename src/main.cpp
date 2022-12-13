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
    restructure material class
    improve display_mode 1
    create 2 buffers and only display the pixels which have changed (less set_color calls)
    or use double buffering so that render() doesn't have to wait until display() is finished
    maybe create scene class and store camera, objects, ... in there

*/

int main() {
  // parse config file
  Config config("config.json");

  // init everything
  PixelBuffer pixelbuffer(&config);
  Camera camera(&config);
  Octree root(&config);
  root.fill("sphere", 100, false);
  root.insert_vertex(Vertex(Vec3f(0.45,0.45,0.45), Material(reflective, RGBi("white"), 0.9, 0.2, 1.0)), false);
  root.insert_vertex(Vertex(Vec3f(0.45,0.55,0.45), Material(reflective, RGBi("white"), 0.9, 0.2, 1.0)), false);
  root.insert_vertex(Vertex(Vec3f(0.55,0.45,0.45), Material(reflective, RGBi("white"), 0.9, 0.2, 1.0)), false);
  root.insert_vertex(Vertex(Vec3f(0.55,0.55,0.45), Material(reflective, RGBi("white"), 0.9, 0.2, 1.0)), false);
  root.insert_vertex(Vertex(Vec3f(0.45,0.45,0.55), Material(reflective, RGBi("white"), 0.9, 0.2, 1.0)), false);
  root.insert_vertex(Vertex(Vec3f(0.45,0.55,0.55), Material(reflective, RGBi("white"), 0.9, 0.2, 1.0)), false);
  root.insert_vertex(Vertex(Vec3f(0.55,0.45,0.55), Material(reflective, RGBi("white"), 0.9, 0.2, 1.0)), false);
  root.insert_vertex(Vertex(Vec3f(0.55,0.55,0.55), Material(reflective, RGBi("white"), 0.9, 0.2, 1.0)), false);
  Renderer renderer(&config, &pixelbuffer, &camera, &root);
  Clock clock(&config);
  Terminal terminal(&config);
  show_cursor(false);

  // main loop
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
    camera.view_angle_z += 0.5*clock.frametime;
    camera.view_angle_changed();
    cam_angle += 0.5*clock.frametime;

    clock.finished_frame();
    clock.display_performance();
  }
  show_cursor(true);
  return 0;
}
