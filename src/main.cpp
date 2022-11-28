#include <iostream>
#include <chrono>
#include "include/terminal.hpp"
#include "include/clock.hpp"
#include "include/pixelbuffer.hpp"
#include "include/camera.hpp"
#include "include/renderer.hpp"
#include "include/octree.hpp"
#include "include/vector.hpp"

/*

  TODO:
    Skybox UV mapping (read from pixels from image)
    create config file and read from that
    fix display bug

*/

int main() {
  /* Create Pixel Buffer */
  PixelBuffer pixelbuffer(0, 0);

  /* Scene */
  Camera camera(Vec3f(0.5, -1., 1.5), Vec3f(0.,1.,0.), Vec3f(0.,0.,1.), 55 );
  Vec3f light(20.,10.,30.);
  Octree root( 0.,0.,0., 1.,1.,1., 4, true ); 
  root.fill("sphere", 100, false);

  /* Init and Setup */
  Renderer renderer(&pixelbuffer, &camera, &root, &light, false);
  Clock clock(60);
  Terminal terminal(terminal_color);
  terminal.show_cursor(false);

  /* Main Loop */
  float cam_angle = 0; // rad
  system("clear");
  while (cam_angle < 4.25*3.14) {
    clock.start();
    renderer.threaded_render();
    clock.finished_render();
    terminal.display(&pixelbuffer);
    clock.finished_display();

    // maybe change this here to something like scene.update()
    camera.view_point.x = sin(cam_angle)*1.5+0.5;
    camera.view_point.y = cos(cam_angle)*1.5+0.5;
    camera.view_direction = (Vec3f(0.5,0.5,0.5) - camera.view_point).normalize();
    camera.view_up = (Vec3f(0.5,0.5,2.5) - camera.view_point).normalize();
    cam_angle += 0.5*clock.frametime;

    clock.finished_frame();
    clock.display_stats();
  }
  terminal.show_cursor(true);
  return 0;
}
