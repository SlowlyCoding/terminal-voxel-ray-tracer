#pragma once
#include <vector>
#include "material.hpp"
#include "octree.hpp"
#include "vertex.hpp"

enum Shape {
  grid,
  sphere,
  cylinder,
  noise
};

/* creates a point cloud which can be inserted into an octree */
/* point_count is not needed for the shapes grid and noise */
void create_point_cloud(
    std::vector<Vertex> *vertices, 
    Shape shape, 
    int point_count, 
    Material *material, 
    Octree *root
);
