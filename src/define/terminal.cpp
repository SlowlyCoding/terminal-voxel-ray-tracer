#include "../include/terminal.hpp"

void Terminal::show_cursor(bool show) {
  if (show) {
    printf("\033[?25h");
  } else {
    printf("\033[?25l");
  }
}

void Terminal::set_foreground_color(int r, int g, int b) {
  printf("\033[38;2;%u;%u;%um", r, g, b);
}
void Terminal::set_background_color(int r, int g, int b) {
  printf("\033[48;2;%u;%u;%um", r, g, b);
}

void Terminal::reset_coloring() {
  std::cout << "\033[0m\t\t";
}

void Terminal::go_to(int x, int y) {
  std::cout << "\033[" << x << ";" << y << "H";
}

void Terminal::display(PixelBuffer *pixelbuffer) {
  switch (display_mode) {
    // terminal_ascii skips every second row of the frame because it cant make use of the half-block
    // every row is still being rendered so it doesn't gain any speed either
    case terminal_ascii:
      // set cursor position to top left corner
      go_to(0,0);
      // go through pixel array
      for (int y=0; y<pixelbuffer->height; y++) {
        for (int x=0; x<pixelbuffer->width; x++) {
          // convert from RGB to grayscale
          float brightness = 0.299*pixelbuffer->pixels[y][x][0] +
                             0.587*pixelbuffer->pixels[y][x][1] +
                             0.114*pixelbuffer->pixels[y][x][2];
          brightness /= 256.; // so that brightness ranges from 0 to 1
          // choose the corresponding brightness character from the grayscale array
          std::cout << grayscale[(int)(brightness*10)];
          // if end of frame reached, go to next line
          if (x == pixelbuffer->width-1) {
            y += 1;
            std::cout << "\n";
          }
        }
      }
      break;

    // the number of calls to set the color can be reduces by checking the pixel color has changed
    case terminal_color:
      go_to(0,0);
      for (int y=0; y<pixelbuffer->height; y++) {
        for (int x=0; x<pixelbuffer->width; x++) {
          if (x == 0) {
            set_foreground_color(pixelbuffer->pixels[y+1][x][0], 
                                 pixelbuffer->pixels[y+1][x][1],
                                 pixelbuffer->pixels[y+1][x][2]);
            set_background_color(pixelbuffer->pixels[y][x][0], 
                                 pixelbuffer->pixels[y][x][1], 
                                 pixelbuffer->pixels[y][x][2]);
          } else {
            // if color of current pixel is not the same as color from last pixel
            if (pixelbuffer->pixels[y+1][x][0] != pixelbuffer->pixels[y+1][x-1][0] && 
                pixelbuffer->pixels[y+1][x][0] != pixelbuffer->pixels[y+1][x-1][1] && 
                pixelbuffer->pixels[y+1][x][0] != pixelbuffer->pixels[y+1][x-1][2]) {
              set_foreground_color(pixelbuffer->pixels[y+1][x][0], 
                                   pixelbuffer->pixels[y+1][x][1],
                                   pixelbuffer->pixels[y+1][x][2]);
            }
            if (pixelbuffer->pixels[y][x][0] != pixelbuffer->pixels[y][x-1][0] && 
                pixelbuffer->pixels[y][x][0] != pixelbuffer->pixels[y][x-1][1] && 
                pixelbuffer->pixels[y][x][0] != pixelbuffer->pixels[y][x-1][2]) {
              set_background_color(pixelbuffer->pixels[y][x][0], 
                                   pixelbuffer->pixels[y][x][1], 
                                   pixelbuffer->pixels[y][x][2]);
            }
          }
          std::cout << "\u2584"; // print lower half block
          if (x == pixelbuffer->width-1) {
            y += 1;
            std::cout << "\n";
          }
        }
      }
      break;
    case image:
      std::cout << "image output not yet handled\n";
      // write to ppm file
      break;
  };
  reset_coloring();
}
