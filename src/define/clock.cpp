#include "../include/clock.hpp"

void Clock::calculate_rendertime() {
  t_render = std::chrono::high_resolution_clock::now();
  rendertime = std::chrono::duration_cast<std::chrono::microseconds>(t_render-t_start).count()/1000000.;
}
void Clock::calculate_displaytime() {
  t_display = std::chrono::high_resolution_clock::now();
  displaytime = std::chrono::duration_cast<std::chrono::microseconds>(t_display-t_render).count()/1000000.;
}
void Clock::calculate_frametime() {
  t_frame = std::chrono::high_resolution_clock::now();
  frametime = std::chrono::duration_cast<std::chrono::microseconds>(t_frame-t_start).count()/1000000.;

  // limiting fps
  if (fps_limit != 0) {
    std::this_thread::sleep_for(std::chrono::microseconds((int)((1000000/fps_limit - frametime*1000000)*0.92)));
    // calculate new frametime
    t_frame = std::chrono::high_resolution_clock::now();
    frametime = std::chrono::duration_cast<std::chrono::microseconds>(t_frame-t_start).count()/1000000.;
  }
  // start clock again 
  t_start = std::chrono::high_resolution_clock::now();
}
void Clock::show_stats(int *pixels, int *window_width, uint64_t *frame) {
  // only update performance stats every 10 frames so that they are readable and dont flicker
  if (*frame % 10 == 0) {
    rendertime_str = std::to_string(rendertime*1000);
    rendertime_str = rendertime_str.substr(0, rendertime_str.find(".")+precision+1);
    displaytime_str = std::to_string(displaytime*1000);
    displaytime_str = displaytime_str.substr(0, displaytime_str.find(".")+precision+1);
    frametime_str = std::to_string(frametime*1000);
    frametime_str = frametime_str.substr(0, frametime_str.find(".")+precision+1);
    
    fps_str = std::string("FPS: ").append(std::to_string((int)(1/frametime))).append(std::string("     "));
    rendertime_str = std::string("Render: ").append(rendertime_str).append(std::string("ms  "));
    displaytime_str = std::string("Display: ").append(displaytime_str).append(std::string("ms  "));
    frametime_str = std::string("Frame: ").append(frametime_str).append(std::string("ms  "));
  }
  // write stats to the framebuffer
  /* for (int i=0; i<fps_str.length(); i++) { */
  /*   pixels[i] = fps_str[i]; */
  /* } */
  /* for (int i=0; i<frametime_str.length(); i++) { */
  /*   pixels[*window_width+i] = frametime_str[i]; */
  /* } */
  /* for (int i=0; i<rendertime_str.length(); i++) { */
  /*   pixels[(*window_width)*2+i] = rendertime_str[i]; */
  /* } */
  /* for (int i=0; i<displaytime_str.length(); i++) { */
  /*   pixels[(*window_width)*3+i] = displaytime_str[i]; */
  /* } */
  std::cout << fps_str << rendertime_str << displaytime_str << frametime_str << std::endl;
}
