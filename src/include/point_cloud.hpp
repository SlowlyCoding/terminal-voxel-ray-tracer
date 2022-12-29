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

void create_point_cloud(
    std::vector<Vertex> *vertices, 
    Shape shape, 
    int point_count, 
    Material *material, 
    Octree *root
);
