#pragma once
#include <iostream>
#include <vector>

class PixelBuffer {
public:
  int width;
  int height;
  std::vector<std::vector<std::vector<int>>> pixels;
  PixelBuffer(int _width, int _height);
};
