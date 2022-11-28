#pragma once
#include <iostream>
#include "clock.hpp"
#include "config.hpp"
#include "pixelbuffer.hpp"

class Terminal {
private:
  int display_mode; 
  char grayscale[11] = " .:-=+*#%@";
public:
  Terminal(Config *config);
  void show_cursor(bool show);
  void set_foreground_color(int r, int g, int b);
  void set_background_color(int r, int g, int b);
  void reset_coloring();
  void go_to(int x, int y);
  void display(PixelBuffer *pb);
};
