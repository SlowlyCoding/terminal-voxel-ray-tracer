#pragma once
#include <iostream>
#include "clock.hpp"
#include "pixelbuffer.hpp"

enum DisplayMode {
  terminal_ascii,
  terminal_color,
  image,
};

class Terminal {
private:
  DisplayMode display_mode; 
  char grayscale[11] = " .:-=+*#%@";
public:
  Terminal(DisplayMode display_mode) : display_mode(display_mode) {};
  void show_cursor(bool show);
  void set_color(int r_back, int g_back, int b_back, int r_fore, int g_fore, int b_fore);
  void reset_coloring();
  void go_to(int x, int y);
  void display(PixelBuffer *pb);
};
