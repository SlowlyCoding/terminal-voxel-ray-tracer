# Terminal Voxel Ray Tracer

Introduction text
This ray tracer only works on unix system, but im sure that with some small tweaks to `terminal.cpp` it would work just as well on windows.

<p align="center">
<img src="https://github.com/SlowlyCoding/voxel_ray_tracer/blob/master/showcase/video.gif">
<img src="https://github.com/SlowlyCoding/voxel_ray_tracer/blob/master/showcase/video2.gif">


## Table of Contents

1. [Installation](#installation)
2. [Configuration](#configuration)

## Installation

```shell
git clone https://github.com/SlowlyCoding/voxel_ray_tracer
cd voxel_ray_tracer
make
```

## Configuration

Everything that is configurable can be found in `config.json`
```cpp
{
  "terminal": {
    // fullscreen enabled
    //    maximum available width and height will be used
    // fullscreen disabled
    //    given width and height will be used 
    "fullscreen": false,
    "width": 120,
    "height": 102,
    // display_mode 0 - ASCII
    // display_mode 1 - colored ANSI
    "display_mode": 1, 
    // limiting FPS
    "fps_limit_enabled": true,
    "fps_limit": 60
  },

  "camera": {
    "view_point": [ 1.5, -0.5, 0.5 ],
    "view_angle_x": 0.01, // pitch (in rad)
    "view_angle_z": 3.1415, // yaw (in rad)
    "fov": 55 // field of view
  }, 

  "octree": {
    // no octree corner can be smaller than 0
    // for example side_length 1.1 would not work with this octree center point
    "center": [ 0.5, 0.5, 0.5 ],
    "side_length": 1,
    // depth defines how deep the octree is
    // 8^depth = maximum number of voxels (capacity)
    // cuberoot(8^depth) = octree side length in voxels
    "depth": 4,
    // same_voxel_size true
    //    voxels will always be stored at the deepest level, 
    //    meaning they will all be of the same size
    "same_voxel_size": true
  },

  "light": {
    "position": [ 20, -10, 30 ]
  },

  "renderer": {
    // grayscale used for display_mode 0
    "grayscale": " .:-=+*#%@",
    "shadows_enabled": true,
    "skybox_enabled": true,
    "skybox": "assets/checkermap2.jpg"
  }
}
```
