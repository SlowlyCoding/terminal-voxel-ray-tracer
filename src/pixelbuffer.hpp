#pragma once
#include "config.hpp"
#include <vector>
#include <unistd.h>
#include <sys/ioctl.h>

class PixelBuffer {
    public:
        int width;
        int height;
        std::vector<std::vector<std::vector<int>>> pixels;
        PixelBuffer(Config& config);
};
