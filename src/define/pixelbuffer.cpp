#include "../include/pixelbuffer.hpp"

PixelBuffer::PixelBuffer(int _width, int _height) {
  width = _width;
  height = _height;
  // height can only be an even number
  if (height % 2 != 0) {
    height -= 1;
  }
  pixels = std::vector<std::vector<std::vector<int>>>(height, std::vector<std::vector<int>>(width, std::vector<int>(3,0)));
}
