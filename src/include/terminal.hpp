#pragma once
#include <iostream>
#include "clock.hpp"

class Terminal {
private:
  int window_width;
  int window_height;
  int *pixels;
public:
  Terminal(int window_width, int window_height, int *pixels) : window_width(window_width), window_height(window_height), pixels(pixels) {};
  void show_cursor(bool show);
  void set_background_color(int r, int g, int b);
  void reset_coloring();
  void go_to(int x, int y);
  void display();
};
