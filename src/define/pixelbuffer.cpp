#include "../include/pixelbuffer.hpp"

PixelBuffer::PixelBuffer(Config *config) {
  if (config->terminal_fullscreen) {
    // get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO,   TIOCGWINSZ, &w);
    width = w.ws_col;
    height = (w.ws_row-3)*2;
  } else {
    width = config->terminal_width;
    height = config->terminal_height;
  }
  // height can only be an even number
  if (height % 2 != 0) {
    height += 1;
  }
  pixels = std::vector<std::vector<std::vector<int>>>(height, std::vector<std::vector<int>>(width, std::vector<int>(3,0)));
}
