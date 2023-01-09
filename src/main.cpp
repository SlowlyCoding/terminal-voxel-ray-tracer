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
    look at fmt library, maybe its faster than cout
    fix octree.fill for noise (its hardcoded rn)
    improve display_mode 2
    create 2 buffers and only display the pixels which have changed (less set_color calls)
    or use double buffering so that render() doesn't have to wait until display() is finished

*/

int main() {
  std::cout << "\n[WASD] - move\n";
  std::cout << "[Mouse] - look around\n";
  std::cout << "[Q/E] - move up and down\n";
  std::cout << "[PageUp/PageDown] - change FOV\n";
  std::cout << "[Space] - change display mode\n";
  std::cout << "[Esc] - quit\n";

  /* parse config file */
  Config config("config.json");

  PixelBuffer pixelbuffer(&config);
  Camera camera(&config);
  Octree octree(&config);

  /* create materials */
  Material glass = new_material_refractive(RGBi("purple"), 0.1, 1.5);
  Material mirror = new_material_reflective(RGBi("white"), 0.1);
  Material color = new_material_standard(RGBi("green"), 0.3, 0.8);

  /* fill octree with a point cloud */
  std::cout << "\nfilling octree...\n";
  octree.fill(Shape::sphere, 15, &mirror, false);
  octree.fill(Shape::sphere, 15, &glass, false);
  octree.fill(Shape::sphere, 25, &color, false);

  /* octree.fill(Shape::sphere, 250, &mirror, false); */
  /* octree.fill(Shape::noise, 0, &color, false); */
  /* octree.fill(Shape::sphere, 50000, &mirror, false); */
  std::cout << octree.root.count_voxels() << " Voxels inserted\n";

  Renderer renderer(&config, &pixelbuffer, &camera, &octree);
  Clock clock(&config);
  Terminal terminal(&config);
  show_cursor(false);

  /* setup done */
  std::cout << "\nPress Enter to start\n";
  std::cin.ignore();

  /* main loop */
  bool quit = false;
  while (!quit) {
    clock.start();
    renderer.threaded_render();
    clock.finished_render();
    terminal.display(&pixelbuffer);
    clock.finished_display();
    quit = terminal.handle_events(&camera, clock.frametime);
    clock.finished_frame();
    clock.display_performance();
  }
  show_cursor(true);
  return 0;
}
