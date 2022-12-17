#include "../include/pixelbuffer.hpp"

PixelBuffer::PixelBuffer(Config *config) {
  // get terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  width = w.ws_col;
  height = (w.ws_row-2)*2;

  if (!config->terminal_fullscreen) {
    // if width and height in config.json are bigger than the terminal size don't use the values from config.json
    if (config->terminal_width < width) {
      width = config->terminal_width;
    }
    if (config->terminal_height < height) {
      height = config->terminal_height;
    }
  }
  // height can only be an even number
  if (height % 2 != 0) {
    height += 1;
  }
  pixels = std::vector<std::vector<std::vector<int>>>(height, std::vector<std::vector<int>>(width, std::vector<int>(3,0)));
}
