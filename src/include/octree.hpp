/* 
  the Octree must have a positive axis aligned bounding box
*/
#pragma once
#include "config.hpp"
#include "intersection_information.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "vertex.hpp"
#include "../include_external/reputeless/PerlinNoise.hpp"
#include <vector>
#include <bitset>
/* if SVO is false points will always be stored inside the octree at the deepest level (height 0) */
/* if SVO is true parent nodes will only be created when 2 points happen to be inside 1 node */
#define SVO 0

enum NodeType {
  Parent,
  Point,
  Leaf
};

class Octree {
  private:
    NodeType type;

    /* data being stored in the octree */
    Vertex v;

    /* bounding box */
    Vec3f center;
    float radius;

    Octree* children[8];

    /* leafes have height 0 and the root node has the maximum depth of the octree as its height */
    int current_height;

    /* declaring leaf node */
    Octree(Vec3f _center, float _radius);
    /* declaring point node */
    Octree(Vec3f _center, float _radius, Vertex _v);
    /* declaring parent/root node */
    Octree(Vec3f _center, float _radius, int _current_height);

    /* checks if a ray intersects the node this function is being called on and assigns */
    /* the t scalar for when the ray enters and exits the node */
    bool ray_hit_node(Ray *ray, float *_t_min, float *_t_max);
  public:
    /* define octree root node from outside (by user) */
    Octree(Config *config);
    /* inserts a vertex into the octree */
    void insert_vertex(Vertex v, bool debug);
    /* fills the octree with voxels using the given parameters */
    void fill(std::string shape, int voxelcount, Material *material, bool debug);
    int count_voxels();
    /* returns closest point node the ray hit */ 
    /* only_solids is used for shadow rays which cant intersect transparent objects */
    bool intersection(Ray *ray, intersection_information *ii, bool only_solids);
};
