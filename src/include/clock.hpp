#pragma once
#include <chrono>
#include <string>
#include <thread>
#include <iostream>

class Clock {
private:
  std::chrono::high_resolution_clock::time_point t_start, t_render, t_display, t_frame; 
  int fps_limit = 0;
  int precision = 1; // how many decimal places the frametimes have
public:
  double rendertime, displaytime, frametime=0.1; // seconds
  std::string fps_str, rendertime_str, displaytime_str, frametime_str;
  Clock() {t_start = std::chrono::high_resolution_clock::now();}; 
  Clock(int fps_limit) : fps_limit(fps_limit) {t_start = std::chrono::high_resolution_clock::now();};
  void calculate_rendertime();
  void calculate_displaytime();
  void calculate_frametime();
  void show_stats(int *pixels, int *window_width, uint64_t *frame);
};


