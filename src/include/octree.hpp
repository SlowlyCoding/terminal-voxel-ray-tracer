/* 
  the Octree must have a positive axis aligned bounding box
*/
#pragma once
#include "config.hpp"
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
/*
  if SVO is false points will always be stored inside the octree at the deepest level (height 0)
  if SVO is true parent nodes will only be created when 2 points happen to be inside 1 node
*/
#define SVO 0

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
    /* std::vector<float> bounding_plane_d; */
    /* float bounding_plane_d[9]; */
    Vec3f center;
    float radius;
    /* std::vector<Octree*> children; */
    Octree* children[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    int max_depth;
    // declaring leaf node
    Octree(Vec3f _center, float _radius);
    // declaring point node
    Octree(Vec3f _center, float _radius, Vertex _v);
    // declaring parent/root node
    Octree(Vec3f _center, float _radius, int _max_depth);
    bool ray_hit_node(Ray *ray, float *_t_min, float *_t_max);
  public:
    // define octree root node from outside (by user)
    Octree(Config *config);
    // inserts a point into the octree by splitting each node which contains more than 1 point, thus creating the Octree
    void insert_vertex(Vertex v, bool debug);
    void fill(std::string shape, int voxelcount, Material *material, bool debug);
    int count_voxels();
    // returns closest point node the ray hit
    bool intersection(Ray *ray, intersection_information *ii, bool only_solids);
};
