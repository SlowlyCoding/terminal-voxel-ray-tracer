#pragma once
#include <unistd.h>
#include <sys/ioctl.h>
#include <vector>

class PixelBuffer {
public:
  int width;
  int height;
  std::vector<std::vector<std::vector<int>>> pixels;
  PixelBuffer(int _width, int _height);
};
