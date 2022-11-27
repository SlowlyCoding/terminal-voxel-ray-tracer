#pragma once
#include <cmath>
#include <chrono>
#include <string>
#include <thread>
#include <iostream>
#include "pixelbuffer.hpp"

class Clock {
private:
  std::chrono::high_resolution_clock::time_point t_start, t_render, t_display, t_frame; 
  int precision = 1; // precision for stats (decimal places)
  double rendertime, displaytime; // seconds
public:
  double frametime; // seconds
  int fps_limit = 0;
  std::string fps_str, rendertime_str, displaytime_str, frametime_str;
  Clock() {}; 
  Clock(int fps_limit) : fps_limit(fps_limit) {};
  void start();
  void finished_render();
  void finished_display();
  void finished_frame();
  void display_stats();
};


