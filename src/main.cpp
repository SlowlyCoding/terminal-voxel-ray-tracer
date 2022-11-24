#include <unistd.h>
#include <sys/ioctl.h>
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
    Skybox UV mapping

*/

int main() {
  /* Get Terminal Size */
  struct winsize w;
  ioctl(STDOUT_FILENO,   TIOCGWINSZ, &w);
  int window_width = w.ws_col;
  int window_height = (w.ws_row-3)*2;

  /* Create Pixel Buffer */
  PixelBuffer pixelbuffer(window_width, window_height);

  /* Scene */
  Camera camera(Vec3f(0.5, -1., 1.5), Vec3f(0.,1.,0.), Vec3f(0.,0.,1.), 65 );
  Vec3f light(20.,10.,30.);
  Octree root( 0.,0.,0., 1.,1.,1., 4, true ); 
  root.fill("sphere", 50, false);

  /* Init and Setup */
  Renderer renderer(window_width, window_height, &pixelbuffer, &camera, &root, &light, false);
  Clock clock;
  Terminal terminal;
  terminal.show_cursor(false);

  /* Main Loop */
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
    camera.view_direction = (Vec3f(0.5,0.5,0.5) - camera.view_point).normalize();
    camera.view_up = (Vec3f(0.5,0.5,2.5) - camera.view_point).normalize();
    cam_angle += 1.5*clock.frametime;

    clock.finished_frame();
    clock.display_stats();
  }
  terminal.show_cursor(true);
  return 0;
}
