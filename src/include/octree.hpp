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

class OctreeNode {
  public:
    /* init empty */
    OctreeNode() : type(Leaf), center(Vec3f()), radius(0) {};
    /* init leaf node */
    OctreeNode(Vec3f center, float radius) : type(Leaf), center(center), radius(radius) {};
    /* init point node */
    OctreeNode(Vec3f center, float radius, Vertex *v) : type(Point), center(center), radius(radius), v(v) {};
    /* init parent/root node */
    OctreeNode(Vec3f _center, float _radius, unsigned int _current_height);

    NodeType type;

    /* data being stored in the octree */
    Vertex *v;

    /* bounding box */
    Vec3f center;
    float radius;

    OctreeNode* children[8];

    void insert_vertex(Vertex *v, unsigned int depth, bool debug);
    int count_voxels();
    /* checks if a ray intersects the node this function is being called on and assigns */
    /* the t scalar for when the ray enters and exits the node */
    bool ray_hit_node(Ray *ray, float *_t_min, float *_t_max);
    /* returns closest point node the ray hit */ 
    /* only_solids is used for shadow rays which cant intersect transparent objects */
    bool intersection(Ray *ray, intersection_information *ii, bool only_solids=false);
};

enum Shape {
  grid,
  sphere,
  cylinder,
  noise
};

class Octree {
  public: 
    OctreeNode root;
    unsigned int depth;
    std::vector<Vertex> vertices;

    Octree(Config *config);
    void fill(Shape shape, int point_count, Material *material, bool debug);
};
