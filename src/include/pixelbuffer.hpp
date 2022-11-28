#pragma once
#include "config.hpp"
#include <unistd.h>
#include <sys/ioctl.h>
#include <vector>

class PixelBuffer {
public:
  int width;
  int height;
  std::vector<std::vector<std::vector<int>>> pixels;
  PixelBuffer(Config *config);
};
