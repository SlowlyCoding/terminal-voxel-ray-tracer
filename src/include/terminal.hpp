#pragma once
#include <iostream>
#include "clock.hpp"
#include "pixelbuffer.hpp"

class Terminal {
public:
  void show_cursor(bool show);
  void set_background_color(int r, int g, int b);
  void set_foreground_color(int r, int g, int b);
  void reset_coloring();
  void go_to(int x, int y);
  void display(PixelBuffer *pb);
};
