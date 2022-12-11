#include "../include/terminal.hpp"

Terminal::Terminal(Config *config) {
  display_mode = config->terminal_display_mode;
  grayscale = config->renderer_grayscale;
}

void show_cursor(bool show) {
  if (show) {
    printf("\033[?25h");
  } else {
    printf("\033[?25l");
  }
}

void set_foreground_color(int r, int g, int b) {
  printf("\033[38;2;%u;%u;%um", r, g, b);
}
void set_background_color(int r, int g, int b) {
  printf("\033[48;2;%u;%u;%um", r, g, b);
}

void reset_coloring() {
  std::cout << "\033[0m\t\t";
}

void go_to(int x, int y) {
  std::cout << "\033[" << x << ";" << y << "H";
}

void Terminal::display(PixelBuffer *pixelbuffer) {
  // set cursor position to top left corner
  go_to(0,0);
  switch (display_mode) {
    /*
      display_mode 0 - ascii
      this display mode skips every second row of the frame because it cant make use of the half-block like 
      display mode 1. Every row is still being rendered so it doesn't gain any speed either
    */
    case 0:
      // go through pixel array
      for (int y=0; y<pixelbuffer->height; y++) {
        for (int x=0; x<pixelbuffer->width; x++) {
          // convert from RGB to grayscale
          float brightness = 0.299*pixelbuffer->pixels[y][x][0] +
                             0.587*pixelbuffer->pixels[y][x][1] +
                             0.114*pixelbuffer->pixels[y][x][2];
          brightness /= 256.; // so that brightness ranges from 0 to 1
          // choose the corresponding brightness character from the grayscale array
          std::cout << grayscale[(int)(brightness*grayscale.length())];
        }
        y += 1;
        std::cout << "\n";
      }
      break;

    /*
      display_mode 1 - colored ANSI
      this display mode displays 2 pixel rows on 1 character row by using the lower half block ( \u2584 )
      the number of calls to set the color can be reduces by checking if the pixel color has changed
    */
    case 1:
      for (int y=0; y<pixelbuffer->height; y++) {
        for (int x=0; x<pixelbuffer->width; x++) {
          // if first pixel of row or if color of current pixel is not the same as color of last pixel
          if ((x == 0) || (pixelbuffer->pixels[y][x] != pixelbuffer->pixels[y][x-1])) {  
            set_background_color(
                pixelbuffer->pixels[y][x][0], 
                pixelbuffer->pixels[y][x][1], 
                pixelbuffer->pixels[y][x][2]);
          }
          if ((x == 0) || (pixelbuffer->pixels[y+1][x] != pixelbuffer->pixels[y+1][x-1])) { 
            set_foreground_color(
                pixelbuffer->pixels[y+1][x][0], 
                pixelbuffer->pixels[y+1][x][1],
                pixelbuffer->pixels[y+1][x][2]);
          }
          std::cout << "\u2584"; // print lower half block
        }
        y += 1;
        std::cout << "\n";
      }
      break;
    default:
      system("clear");
      std::cout << "display_modes:\n0 - ascii\n1 - colored ANSI\n";
  };
  reset_coloring();
}
