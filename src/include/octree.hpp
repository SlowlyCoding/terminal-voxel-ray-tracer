/* 
  The Octree must have a positive axis aligned bounding box
*/
#pragma once
#include "intersection_information.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "vertex.hpp"
#include <vector>
#include <bitset>
#define X_MIN 0
#define Y_MIN 1
#define Z_MIN 2
#define X_MID 3
#define Y_MID 4
#define Z_MID 5
#define X_MAX 6
#define Y_MAX 7
#define Z_MAX 8

enum NodeType {
  Parent,
  Point,
  Leaf
};

class Octree {
  private:
    NodeType type;
    Vertex v;
    // stores all the d parameters of the 9 bounding planes
    std::vector<float> bounding_plane_d;
    std::vector<Octree*> children;
    int max_depth;
    bool same_voxel_size;
    // declaring leaf node
    Octree(
        float x_min, float y_min, float z_min, // bottom left front
        float x_max, float y_max, float z_max  // top right back
        );
    // declaring point node
    Octree(
        float x_min, float y_min, float z_min,
        float x_max, float y_max, float z_max,
        Vertex v
        );
    // inserts a point into the octree by splitting each node which contains more than 1 point, thus creating the Octree
    void insert_vertex(Vertex v, bool debug);
    bool ray_hit_node(Ray *ray, float *_t_min, float *_t_max);
    // returns closest point node the ray hit
  public:
    // declaring parent/root node
    Octree(
        float x_min, float y_min, float z_min,
        float x_max, float y_max, float z_max,
        int max_depth, bool same_voxel_size
        );
    void fill(std::string shape, int voxelcount, bool debug);
    int count_voxels();
    bool intersection(Ray *ray, intersection_information *ii);
};
