#include "../include/terminal.hpp"

void Terminal::show_cursor(bool show) {
  if (show) {
    printf("\033[?25h");
  } else {
    printf("\033[?25l");
  }
}

void Terminal::set_color(int rf, int gf, int bf, int rb, int gb, int bb) {
  printf("\033[38;2;%d;%d;%d;48;2;%d;%d;%dm", rf, gf, bf, rb, gb, bb);
}

void Terminal::reset_coloring() {
  std::cout << "\033[0m\t\t";
}

void Terminal::go_to(int x, int y) {
  std::cout << "\033[" << x << ";" << y << "H";
}

void Terminal::display(PixelBuffer *pixelbuffer) {
  // set cursor position to top left corner
  go_to(0,0);
  // go through pixel array
  for (int y=0; y<pixelbuffer->height; y++) {
    for (int x=0; x<pixelbuffer->width; x++) {
        set_color(pixelbuffer->pixels[y+1][x][0], pixelbuffer->pixels[y+1][x][1], pixelbuffer->pixels[y+1][x][2], pixelbuffer->pixels[y][x][0], pixelbuffer->pixels[y][x][1], pixelbuffer->pixels[y][x][2]);
        std::cout << "\u2584"; // print lower half block
      // if end of frame is reached, start new line
      if (x == pixelbuffer->width-1) {
        y += 1;
        std::cout << "\n";
      }
    }
  }
  reset_coloring();
}
