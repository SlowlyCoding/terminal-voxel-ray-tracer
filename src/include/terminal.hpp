#pragma once
#include <iostream>
#include <string>
#include "clock.hpp"
#include "config.hpp"
#include "pixelbuffer.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

class Terminal {
private:
  int display_mode; 
  std::string grayscale = " .:-=+*#%@";
public:
  Terminal(Config *config);
  void display(PixelBuffer *pb);
};
void show_cursor(bool show);
void go_to(int x, int y);
void reset_coloring();
void set_foreground_color(int r, int g, int b);
void set_background_color(int r, int g, int b);
