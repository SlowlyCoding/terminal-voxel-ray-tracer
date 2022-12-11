#pragma once
#include "config.hpp"
#include <vector>

#ifdef linux
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

class PixelBuffer {
public:
  int width;
  int height;
  std::vector<std::vector<std::vector<int>>> pixels;
  PixelBuffer(Config *config);
};
