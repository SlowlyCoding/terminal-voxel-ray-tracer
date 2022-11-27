#pragma once
#include <iostream>
#include "clock.hpp"
#include "pixelbuffer.hpp"

class Terminal {
public:
  void show_cursor(bool show);
  void set_color(int rf, int gf, int bf, int rb, int gb, int bb);
  void reset_coloring();
  void go_to(int x, int y);
  void display(PixelBuffer *pb);
};
