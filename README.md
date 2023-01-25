# Terminal Voxel Ray Tracer

I started this project to learn more about voxels, a bit of research led me to sparse voxel octrees for storing voxels that don't change their position a lot, which is what I used here.

I had already had some experience with console graphics, so I made this ray tracer output to the terminal as well.

This ray tracer only works on unix system, but I'm sure that with some small tweaks to `terminal.cpp` and `pixelbuffer.cpp` it would work just as well on Windows.

<p align="center">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/showcase.gif">

The display mode can be changed by pressing the spacebar

<p align="center">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/ANSI.png">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/ASCII_colored.png">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/ASCII.png">


## Table of Contents

1. [Requirements](#requirements)
2. [Installation](#installation)
3. [Configuration](#configuration)

## Requirements

[SFML](https://www.sfml-dev.org/download.php) is required for this to work

SFML is only used to get keyboard and mouse input, there is no SFML window.

## Installation

```shell
git clone https://github.com/SlowlyCoding/terminal_voxel_ray_tracer
cd terminal_voxel_ray_tracer
make
```

## Configuration

Everything that is configurable can be found in `config.json`

An explanation of the config file:
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
    // display_mode 1 - colored ASCII
    // display_mode 2 - colored ANSI
    "display_mode": 2, 
    // limiting FPS
    "fps_limit_enabled": true,
    "fps_limit": 60
  },

  "camera": {
    "view_point": [ 1.5, -0.5, 0.5 ],
    "view_angle_x": 0.01, // pitch (in rad)
    "view_angle_z": 3.1415, // yaw (in rad)
    "fov": 55, // field of view
    "speed": 0.5, // speed of camera movement (WASD and QE)
    "mouse_sensitivity": 0.1
  }, 

  "octree": {
    // no octree corner can be smaller than 0
    // for example side_length 1.1 would not work with this octree center point
    "center": [ 0.5, 0.5, 0.5 ],
    "side_length": 1,
    // depth defines how deep the octree is
    // 8^depth = maximum number of voxels (capacity)
    // cuberoot(8^depth) = octree side length in voxels
    "depth": 4
  },

  "lighting": {
    // [ x, y, z, light intensity ]
    "pointlights": [[ 10, 10, 20, 2 ],
                    [ -10, -10, -20, 2 ]]
  },

  "renderer": {
    "max_ray_bounces": 5,
    // ASCII grayscale used for display_mode 0 and 1
    "grayscale": " .:-=+*#%@"
  }

  "skybox": {
    "enabled": true,
    "file": "assets/checkermap.jpg"
  }
}
```
