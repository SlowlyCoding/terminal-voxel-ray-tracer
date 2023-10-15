#include "../include/octree.hpp"

OctreeNode::OctreeNode(Vec3f _center, float _radius, unsigned int _current_height) {
  type = Parent;
  center = _center;
  radius = _radius;
  for (int i=0; i<8; i++) {
    std::bitset<3> child_bitset(i);
    children[i] = new OctreeNode(
        Vec3f(
          child_bitset[0] ? center.x+radius/2.0f : center.x-radius/2.0f,
          child_bitset[1] ? center.y+radius/2.0f : center.y-radius/2.0f,
          child_bitset[2] ? center.z+radius/2.0f : center.z-radius/2.0f
        ), 
        radius/2.0f
    );
  }
}
Octree::Octree(Config *config) {
  if (config->octree_center.x - config->octree_side_length/2.0f < 0.0f ||
      config->octree_center.y - config->octree_side_length/2.0f < 0.0f ||
      config->octree_center.z - config->octree_side_length/2.0f < 0.0f) return;
  depth = config->octree_depth;
  root = OctreeNode(
      config->octree_center,
      config->octree_side_length*0.5f,
      99
  );
}


void OctreeNode::insert_vertex(Vertex *v, unsigned int depth, bool debug) {
  // check if the tree is deep enough
  if (depth == 0) {
    if (debug) std::cout << "Max tree depth reached\n";
    return;
  }
  // check if point is inside root bounding box
  if (
      v->point.x < center.x-radius || v->point.x > center.x+radius ||
      v->point.y < center.y-radius || v->point.y > center.y+radius ||
      v->point.z < center.z-radius || v->point.z > center.z+radius
     ) {
    if (debug) std::cout << "Point insertion failed, point outside nodes bounding box\n";
    return;
  }
  // find out which child the point is part of
  int child = 0;
  if (v->point.x >= center.x) {
    child |= 1;
  }
  if (v->point.y >= center.y) {
    child |= 2;
  }
  if (v->point.z >= center.z) {
    child |= 4;
  }
  if (debug) std::cout << "  " << depth << " tree height, child " << child << " - ";

  // child is parent node -> go deeper into octree
  if (children[child]->type == Parent) {
    if (debug) std::cout << "is Parent Node\n";
    children[child]->insert_vertex(v, depth-1, debug);
    return;
  }
  // if voxels can be of different sizes (SVO true)
  // child is leaf node -> set node to point node
  //
  // if voxels are set to be of the same size (SVO false)
  // child is leaf node -> set node to parent node until max tree depth reached, 
  //                       then set to point node
  else if (children[child]->type == Leaf) {
    if (debug) std::cout << "was Leaf Node, ";
    delete children[child];
    std::bitset<3> child_bitset(child);
    if (!SVO) {
      if (depth > 1) {
        children[child] = new OctreeNode(
            Vec3f(
              child_bitset[0] ? center.x+radius/2.0f : center.x-radius/2.0f,
              child_bitset[1] ? center.y+radius/2.0f : center.y-radius/2.0f,
              child_bitset[2] ? center.z+radius/2.0f : center.z-radius/2.0f
            ), 
            radius/2.0f,
            1-1
        );
        if (debug) std::cout << "is Parent Node\n";
        children[child]->insert_vertex(v, depth-1, debug);
      } else {
        children[child] = new OctreeNode(
            Vec3f(
              child_bitset[0] ? center.x+radius/2.0f : center.x-radius/2.0f,
              child_bitset[1] ? center.y+radius/2.0f : center.y-radius/2.0f,
              child_bitset[2] ? center.z+radius/2.0f : center.z-radius/2.0f
            ), 
            radius/2.0f,
            v
        );
        if (debug) std::cout << "is Point Node\n";
      }
    } else {
      children[child] = new OctreeNode(
          Vec3f(
            child_bitset[0] ? center.x+radius/2.0f : center.x-radius/2.0f,
            child_bitset[1] ? center.y+radius/2.0f : center.y-radius/2.0f,
            child_bitset[2] ? center.z+radius/2.0f : center.z-radius/2.0f
          ), 
          radius/2.0f,
          v
      );
      if (debug) std::cout << "is Point Node\n";
    }
    return;
  }
  // child is point node -> safe point that is already present, 
  //                        delete point node and make it a parent node, 
  //                        insert both points into parent node
  else {
    if (!SVO) {
      if (debug) std::cout << "is already Point Node\n";
      return;
    }
    if (debug) std::cout << "was Point Node, ";
    Vertex *v_present = children[child]->v;
    delete children[child];
    children[child] = nullptr;
    std::bitset<3> child_bitset(child);
    children[child] = new OctreeNode(
        Vec3f(
          child_bitset[0] ? center.x+radius/2.0f : center.x-radius/2.0f,
          child_bitset[1] ? center.y+radius/2.0f : center.y-radius/2.0f,
          child_bitset[2] ? center.z+radius/2.0f : center.z-radius/2.0f
        ), 
        radius/2.0f,
        1-1
    );
    if (debug) std::cout << "is Parent Node\n  Inserting Point 1\n";
    children[child]->insert_vertex(v_present, depth-1, debug);
    if (debug) std::cout << "  Inserting Point 2\n";
    children[child]->insert_vertex(v, depth-1, debug);
  }
}


int OctreeNode::count_voxels() {
  int voxel_count = 0;
  for (int child=0; child<8; child++) {
    if (children[child]->type == Parent) {
      voxel_count += children[child]->count_voxels();
    }
    else if (children[child]->type == Point) {
      voxel_count++;
    }
  }
  return voxel_count;
}

bool OctreeNode::ray_hit_node(Ray *ray, float *_t_min, float *_t_max) {
  float t_min = ray->min_t; 
  float t_max = ray->max_t; 

  if (ray->direction.x != 0.) {
    float t_x0 = (center.x-radius - ray->origin.x) / ray->direction.x;
    float t_x1 = (center.x+radius - ray->origin.x) / ray->direction.x;
    t_min = std::fmax(t_min, std::fmin(t_x0, t_x1));
    t_max = std::fmin(t_max, std::fmax(t_x0, t_x1));
  }
  if (ray->direction.y != 0.) {
    float t_y0 = (center.y-radius - ray->origin.y) / ray->direction.y;
    float t_y1 = (center.y+radius - ray->origin.y) / ray->direction.y;
    t_min = std::fmax(t_min, std::fmin(t_y0, t_y1));
    t_max = std::fmin(t_max, std::fmax(t_y0, t_y1));
  }
  if (ray->direction.z != 0.) {
    float t_z0 = (center.z-radius - ray->origin.z) / ray->direction.z;
    float t_z1 = (center.z+radius - ray->origin.z) / ray->direction.z;
    t_min = std::fmax(t_min, std::fmin(t_z0, t_z1));
    t_max = std::fmin(t_max, std::fmax(t_z0, t_z1));
  }

  if (t_min < t_max) {
    *_t_min = t_min;
    *_t_max = t_max;
    return true;
  }
  return false;
}

bool OctreeNode::intersection(Ray *ray, intersection_information *ii, bool only_solids) {
  // first check if ray hits octree root
  float t_min, t_max;
  if (ray_hit_node(ray, &t_min, &t_max)) {
    /* generate child penetration order */
    // ray direction mask (vmask)
    std::bitset<3> vmask;
    vmask[0] = ray->direction.x < 0;
    vmask[1] = ray->direction.y < 0;
    vmask[2] = ray->direction.z < 0;
    // child order array
    int child_order[8];
    for (int i=0; i<8; i++) {
      std::bitset<3> temp(i);
      child_order[i] = (int)((temp ^ vmask).to_ulong());
    }
    // go through the child order
    float t_min_child, t_max_child;
    for (int i=0; i<8; i++) {
      // if the ray hits the child, check what type the child is
      if (children[child_order[i]]->ray_hit_node(ray, &t_min_child, &t_max_child)) {
        // child is parent node -> go deeper into octree
        if (children[child_order[i]]->type == Parent) {
          bool intersected = children[child_order[i]]->intersection(ray, ii, only_solids);
          if (intersected) return true;
          // if nothing returned true, continue with next child
        }
        // child is point node -> return intersection information
        else if (children[child_order[i]]->type != Leaf) {
          if (only_solids && children[child_order[i]]->v->material->type == refractive) {
            continue;
          }
          // check if view_point is inside the voxel
          if (t_min_child != 0.0) {
            ii->inside_voxel = false;
            ii->t = t_min_child;
            ii->point = ray->point(t_min_child);
          } else {
            ii->inside_voxel = true;
            ii->t = t_max_child;
            ii->point = ray->point(t_max_child);
          }
          ii->material = children[child_order[i]]->v->material;
          Vec3f cti = (ii->point - children[child_order[i]]->center).normalize(); // center to intersection (cti)
          if (std::abs(cti.x) >= std::abs(cti.y)) {
            if (std::abs(cti.x) >= std::abs(cti.z)) {
              ii->normal = Vec3f(round(cti.x),0.,0.); 
            } else {
              ii->normal = Vec3f(0.,0.,round(cti.z)); 
            }
          } else {
            if (std::abs(cti.y) >= std::abs(cti.z)) {
              ii->normal = Vec3f(0.,round(cti.y),0.); 
            } else {
              ii->normal = Vec3f(0.,0.,round(cti.z)); 
            }
          }
          if (t_min_child == 0.0) {
            ii->normal = ii->normal*-1.f;
          }
          return true;
        }
        // child is leaf node -> do nothing and continue with next child
      }
    }
  }
  return false;
}

void Octree::create_vertices(Shape shape, int point_count, Material *material, bool debug) {
  /* first fill the vertices array with the point cloud using the given parameters */
  switch (shape) {
    case grid: {
      int half_side_length_in_voxels = cbrt(pow(8,depth))*0.5f;
      for (int z=1; z<=half_side_length_in_voxels; z++) {
        for (int y=1; y<=half_side_length_in_voxels; y++) {
          for (int x=1; x<=half_side_length_in_voxels; x++) {
            vertices.push_back(
                Vertex(
                  Vec3f(x/(float)half_side_length_in_voxels,y/(float)half_side_length_in_voxels,z/(float)half_side_length_in_voxels),
                  material
                  ));
          }
        }
      }
      break;
               }
    case sphere: {
      for (int i=0; i<point_count; i++) {
        float angle1 = float(rand())/float((RAND_MAX)) * 2.*3.1415;
        float angle2 = float(rand())/float((RAND_MAX)) * 3.1415;
        vertices.push_back(
            Vertex(
              Vec3f(
                root.radius*cos(angle1)*sin(angle2)+root.center.x, 
                root.radius*sin(angle1)*sin(angle2)+root.center.y, 
                root.radius*cos(angle2)+root.center.z),
              material
              ));
      }
      break;
                 }
    case cylinder: {
      for (int i=0; i<point_count; i++) {
        float angle = float(rand())/float((RAND_MAX)) * 2.*3.1415;
        float height = float(rand())/float((RAND_MAX)) * root.radius*2.0f;
        vertices.push_back(
            Vertex(
              Vec3f(
                cos(angle)*root.radius+root.center.x, 
                sin(angle)*root.radius+root.center.y, 
                height),
                material
              ));
      }
      break;
                   }
      case noise: {
        const siv::PerlinNoise::seed_type seed = 123456u;
        const siv::PerlinNoise perlin{ seed };
        double noise;
        float side_length_in_voxels = cbrt(pow(8, depth));
        for (int y=0; y<side_length_in_voxels; y++) {
          for (int x=0; x<side_length_in_voxels; x++) {
            noise = perlin.octave2D_01(x/side_length_in_voxels, y/side_length_in_voxels, 5);
            while (noise > 0) {
              vertices.push_back(Vertex(Vec3f(x/side_length_in_voxels, y/side_length_in_voxels, noise), material));
              noise -= 1.0f/side_length_in_voxels;
            }
          }
        }
        break;
                  }
  }
}

void Octree::fill(bool debug) {
  /* then fill the octree with the vertices */
  for (int i=0; i<vertices.size(); i++) {
    if (debug) std::cout << "Inserting Vertex " << i << ": ";
    if (debug) vertices[i].point.values();
    root.insert_vertex(&vertices[i], depth, debug);
  }
}
