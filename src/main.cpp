#include <iostream>
#include "include/config.hpp"
#include "include/terminal.hpp"
#include "include/clock.hpp"
#include "include/pixelbuffer.hpp"
#include "include/camera.hpp"
#include "include/renderer.hpp"
#include "include/point_cloud.hpp"
#include "include/octree.hpp"
#include "include/vector.hpp"
#define PI 3.14159265

/*

  TODO:
    look at fmt library, maybe its faster than cout
    fix create_point_cloud for noise (its hardcoded rn)
    improve display_mode 2
    create 2 buffers and only display the pixels which have changed (less set_color calls)
    or use double buffering so that render() doesn't have to wait until display() is finished
    maybe create scene class and store camera, objects, ... in there

  TODO octree:
  main goal: make octree node smaller
    maybe store octree nodes and vertices somewhere outside in a vector, 
    only store index of the vertex being stored in a node and a pointer to the vector

    sizeof(Vertex) = 24;
    sizeof(int) + sizeof(&std::vector) = 4 + 8;

*/

int main() {
  /* parse config file */
  Config config("config.json");

  PixelBuffer pixelbuffer(&config);
  Camera camera(&config);
  Octree root(&config);

  /* create materials */
  Material glass = new_material_refractive(RGBi("purple"), 0.2, 1.5);
  Material mirror = new_material_reflective(RGBi("white"), 0.2);
  Material color = new_material_standard(RGBi("green"), 0.3, 0.8);

  /* create point cloud */
  std::cout << "\ncreating point cloud..." << std::endl;
  std::vector<Vertex> vertices;
  create_point_cloud(&vertices, Shape::sphere, 50000, &color, &root);

  /* fill octree with said point cloud */
  std::cout << "filling octree..." << std::endl;
  for (int i=0; i<vertices.size(); i++) {
    root.insert_vertex(&vertices[i], false);
  }
  std::cout << root.count_voxels() << " Voxels inserted\n";

  Renderer renderer(&config, &pixelbuffer, &camera, &root);
  Clock clock(&config);
  Terminal terminal(&config);
  show_cursor(false);

  /* setup done */
  std::cout << "\nPress Enter to start\n";
  std::cin.ignore();

  /* main loop */
  float cam_angle = 0; // rad
  float cam_speed = 0.3;
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
    camera.view_angle_z += cam_speed*clock.frametime;
    camera.view_angle_changed();
    cam_angle += cam_speed*clock.frametime;

    clock.finished_frame();
    clock.display_performance();
  }
  show_cursor(true);
  return 0;
}
