#include "../include/terminal.hpp"

void Terminal::show_cursor(bool show) {
  if (show) {
    printf("\033[?25h");
  } else {
    printf("\033[?25l");
  }
}

void Terminal::set_background_color(int r, int g, int b) {
  std::cout << "\033[48;2;" << r << ";" << g << ";" << b << "m";
  /* std::cout << "\033[38;2;32;150;100m"; // background color: \033[ 38;2;(r);(g);(b) m */
}

void Terminal::reset_coloring() {
  std::cout << "\033[0m\t\t";
}

void Terminal::go_to(int x, int y) {
  printf("\033[%d;%dH",x,y);
}

void Terminal::display() {
  // go through pixel array
  for (int i=0; i<window_width*window_height*3; i+=3) {
    set_background_color(pixels[i], pixels[i+1], pixels[i+2]);
    std::cout << " ";
    // if we have reached the end of the frame, start new line
    if (i % window_width*3 == 0 && i != 0) {
      std::cout << "\n";
    }
  }
  // set cursor position to top left corner
  go_to(0,0);
}
