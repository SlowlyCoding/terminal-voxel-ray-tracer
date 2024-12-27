# Terminal Voxel Ray Tracer

<p align="center">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/dragon1.png">

I started this project to learn more about voxels, a bit of research led me to sparse voxel octrees for storing voxels 
that don't change their position much, which is what I initially used. Later on, I learned how voxels can be stored efficiently 
using only 1 byte material indices, meaning a voxel object with $1000^3$ voxels could be stored using just 1GB of RAM. 
So, I scrapped my octree implementation (which can still be found in some earlier commits) and wrote a new voxel management system.

I tried my best to make this ray tracer as fast and memory-efficient as possible so that it can run on the CPU. 

This ray tracer only works on Unix systems, but I'm sure that with a few minor tweaks to `terminal.cpp` and `pixelbuffer.cpp`, it would work just as well on Windows.

## Table of Contents

1. [Requirements](#requirements)
2. [Installation](#installation)
3. [Configuration](#configuration)
4. [Showcase](#showcase)

## Requirements

[SFML 3](https://www.sfml-dev.org/download.php) is required for this to work

SFML is only used to get keyboard and mouse input, there is no SFML window.

## Installation

```shell
git clone https://github.com/SlowlyCoding/terminal_voxel_ray_tracer
cd terminal_voxel_ray_tracer
```
```shell
make
./main
```

## Configuration

Everything that is configurable can be found in `config.json`. 
I added some comments here for clarification, but you can simply experiment with the values and see what happens :)
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
    "view_angle_x": 0.5, // pitch (in rad)
    "view_angle_z": 3.1415, // yaw (in rad)
    "fov": 70, // field of view
    "speed": 3, // speed of camera movement (WASD and QE)
    "mouse_sensitivity": 0.1,
    // changes up/down direction of mouse input
    "invert_mouse": true
  }, 

  "object": {
    "model_path": "assets/model/dragon.vox",
    "location": [0,0,0],
    "voxel_size": 0.05,
    // check colors in main.cpp line 26-30 or create your own
    "color_index": 3
  },

  "renderer": {
    // how many new rays are shot from each hitpoint to calculate
    // ambient occlusion (the more, the less noise, at the cost of performance)
    "ambient_occlusion_samples": 20,
    "max_ray_bounces": 5,
    // ASCII grayscale used for display_mode 0 and 1
    "grayscale": " .:-=+*#%@"
  }

  "skybox": {
    "enabled": true,
    "file": "assets/skybox/mountains.png"
  }
}
```

## Showcase

Dragon with mirror voxels (40K voxels)
<p align="center">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/dragon2.png">

The whole image can be displayed using ASCII characters by pressing the spacebar
<p align="center">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/teapot1.png">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/teapot2.png">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/teapot3.png">

Sphere containing 8.8 million voxels (16MB)
<p align="center">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/sphere1.png">
<img src="https://github.com/SlowlyCoding/terminal-voxel-ray-tracer/blob/master/showcase/sphere2.png">
