#include "../include/octree.hpp"

Octree::Octree(
    float x_min, float y_min, float z_min,
    float x_max, float y_max, float z_max
    ) {
  type = Leaf;
  bounding_plane_d.assign(9, 0.);
  bounding_plane_d[X_MIN] = x_min;
  bounding_plane_d[Y_MIN] = y_min;
  bounding_plane_d[Z_MIN] = z_min;
  bounding_plane_d[X_MID] = (x_min+x_max)/2.;
  bounding_plane_d[Y_MID] = (y_min+y_max)/2.;
  bounding_plane_d[Z_MID] = (z_min+z_max)/2.;
  bounding_plane_d[X_MAX] = x_max;
  bounding_plane_d[Y_MAX] = y_max;
  bounding_plane_d[Z_MAX] = z_max;
}
Octree::Octree(
    float x_min, float y_min, float z_min,
    float x_max, float y_max, float z_max,
    Vertex _v
    ) {
  type = Point;
  v = _v;
  bounding_plane_d.assign(9, 0.);
  bounding_plane_d[X_MIN] = x_min;
  bounding_plane_d[Y_MIN] = y_min;
  bounding_plane_d[Z_MIN] = z_min;
  bounding_plane_d[X_MID] = (x_min+x_max)/2.;
  bounding_plane_d[Y_MID] = (y_min+y_max)/2.;
  bounding_plane_d[Z_MID] = (z_min+z_max)/2.;
  bounding_plane_d[X_MAX] = x_max;
  bounding_plane_d[Y_MAX] = y_max;
  bounding_plane_d[Z_MAX] = z_max;
}
Octree::Octree(
    float x_min, float y_min, float z_min,
    float x_max, float y_max, float z_max,
    int _max_depth, bool _same_voxel_size
    ) {
  type = Parent;
  bounding_plane_d.assign(9, 0.);
  bounding_plane_d[X_MIN] = x_min;
  bounding_plane_d[Y_MIN] = y_min;
  bounding_plane_d[Z_MIN] = z_min;
  bounding_plane_d[X_MID] = (x_min+x_max)/2.;
  bounding_plane_d[Y_MID] = (y_min+y_max)/2.;
  bounding_plane_d[Z_MID] = (z_min+z_max)/2.;
  bounding_plane_d[X_MAX] = x_max;
  bounding_plane_d[Y_MAX] = y_max;
  bounding_plane_d[Z_MAX] = z_max;
  children.assign(8, nullptr);
  for (int i=0; i<8; i++) {
    std::bitset<3> child_bitset(i);
    children[i] = new Octree(
        (child_bitset[0]) ? bounding_plane_d[X_MID] : bounding_plane_d[X_MIN],
        (child_bitset[1]) ? bounding_plane_d[Y_MID] : bounding_plane_d[Y_MIN],
        (child_bitset[2]) ? bounding_plane_d[Z_MID] : bounding_plane_d[Z_MIN],
        (child_bitset[0]) ? bounding_plane_d[X_MAX] : bounding_plane_d[X_MID],
        (child_bitset[1]) ? bounding_plane_d[Y_MAX] : bounding_plane_d[Y_MID],
        (child_bitset[2]) ? bounding_plane_d[Z_MAX] : bounding_plane_d[Z_MID]
        );
  }
  max_depth = _max_depth;
  same_voxel_size = _same_voxel_size;
}
Octree::Octree(Config *config) {
  type = Parent;
  bounding_plane_d.assign(9, 0.);
  float half_side = config->octree_side_length/2.;
  if (config->octree_center.x - half_side < 0. ||
      config->octree_center.y - half_side < 0. ||
      config->octree_center.z - half_side < 0.) { return; }
  bounding_plane_d[X_MIN] = config->octree_center.x-half_side;
  bounding_plane_d[Y_MIN] = config->octree_center.y-half_side;
  bounding_plane_d[Z_MIN] = config->octree_center.z-half_side;
  bounding_plane_d[X_MID] = config->octree_center.x;
  bounding_plane_d[Y_MID] = config->octree_center.y;
  bounding_plane_d[Z_MID] = config->octree_center.z;
  bounding_plane_d[X_MAX] = config->octree_center.x+half_side;
  bounding_plane_d[Y_MAX] = config->octree_center.y+half_side;
  bounding_plane_d[Z_MAX] = config->octree_center.z+half_side;
  children.assign(8, nullptr);
  for (int i=0; i<8; i++) {
    std::bitset<3> child_bitset(i);
    children[i] = new Octree(
        (child_bitset[0]) ? bounding_plane_d[X_MID] : bounding_plane_d[X_MIN],
        (child_bitset[1]) ? bounding_plane_d[Y_MID] : bounding_plane_d[Y_MIN],
        (child_bitset[2]) ? bounding_plane_d[Z_MID] : bounding_plane_d[Z_MIN],
        (child_bitset[0]) ? bounding_plane_d[X_MAX] : bounding_plane_d[X_MID],
        (child_bitset[1]) ? bounding_plane_d[Y_MAX] : bounding_plane_d[Y_MID],
        (child_bitset[2]) ? bounding_plane_d[Z_MAX] : bounding_plane_d[Z_MID]
        );
  }
  max_depth = config->octree_depth;
  same_voxel_size = config->octree_same_voxel_size;
}

void Octree::insert_vertex(Vertex v, bool debug) {
  // check if the tree is deep enough
  if (max_depth == 0) {
    if (debug) std::cout << "Max tree depth reached\n";
    return;
  }
  // check if point is inside root bounding box
  if (
      v.point.x < bounding_plane_d[X_MIN] || v.point.x > bounding_plane_d[X_MAX] ||
      v.point.y < bounding_plane_d[Y_MIN] || v.point.y > bounding_plane_d[Y_MAX] ||
      v.point.z < bounding_plane_d[Z_MIN] || v.point.z > bounding_plane_d[Z_MAX]
     ) {
    if (debug) std::cout << "Point insertion failed, point outside nodes bounding box\n";
    return;
  }
  // find out which child the point is part of
  int child;
  if (v.point.x <= bounding_plane_d[X_MID]){
    if (v.point.y <= bounding_plane_d[Y_MID]){
      if (v.point.z <= bounding_plane_d[Z_MID]) child = 0;
      else child = 4;
    } else {
      if (v.point.z <= bounding_plane_d[Z_MID]) child = 2;
      else child = 6;
    }
  } else {
    if (v.point.y <= bounding_plane_d[Y_MID]){
      if (v.point.z <= bounding_plane_d[Z_MID]) child = 1;
      else child = 5;
    } else {
      if (v.point.z <= bounding_plane_d[Z_MID]) child = 3;
      else child = 7;
    }
  }
  if (debug) std::cout << "  " << max_depth << " tree height, child " << child << " - ";

  // child is parent node -> go deeper into octree
  if (children[child]->type == Parent) {
    if (debug) std::cout << "is Parent Node\n";
    children[child]->insert_vertex(v, debug);
    return;
  }
  // if voxels can be of different sizes
  // child is leaf node -> set node to point node
  //
  // if voxels are set to be of the same size
  // child is leaf node -> set node to parent node until max tree depth reached, 
  //                       then set to point node
  else if (children[child]->type == Leaf) {
    if (debug) std::cout << "was Leaf Node, ";
    delete children[child];
    std::bitset<3> child_bitset(child);
    if (same_voxel_size) {
      if (max_depth > 1) {
        children[child] = new Octree(
            (child_bitset[0]) ? bounding_plane_d[X_MID] : bounding_plane_d[X_MIN],
            (child_bitset[1]) ? bounding_plane_d[Y_MID] : bounding_plane_d[Y_MIN],
            (child_bitset[2]) ? bounding_plane_d[Z_MID] : bounding_plane_d[Z_MIN],
            (child_bitset[0]) ? bounding_plane_d[X_MAX] : bounding_plane_d[X_MID],
            (child_bitset[1]) ? bounding_plane_d[Y_MAX] : bounding_plane_d[Y_MID],
            (child_bitset[2]) ? bounding_plane_d[Z_MAX] : bounding_plane_d[Z_MID], max_depth-1, same_voxel_size);
        if (debug) std::cout << "is Parent Node\n";
        children[child]->insert_vertex(v, debug);
      } else {
        children[child] = new Octree(
            (child_bitset[0]) ? bounding_plane_d[X_MID] : bounding_plane_d[X_MIN],
            (child_bitset[1]) ? bounding_plane_d[Y_MID] : bounding_plane_d[Y_MIN],
            (child_bitset[2]) ? bounding_plane_d[Z_MID] : bounding_plane_d[Z_MIN],
            (child_bitset[0]) ? bounding_plane_d[X_MAX] : bounding_plane_d[X_MID],
            (child_bitset[1]) ? bounding_plane_d[Y_MAX] : bounding_plane_d[Y_MID],
            (child_bitset[2]) ? bounding_plane_d[Z_MAX] : bounding_plane_d[Z_MID], v);
        if (debug) std::cout << "is Point Node\n";
      }
    } else {
      children[child] = new Octree(
          (child_bitset[0]) ? bounding_plane_d[X_MID] : bounding_plane_d[X_MIN],
          (child_bitset[1]) ? bounding_plane_d[Y_MID] : bounding_plane_d[Y_MIN],
          (child_bitset[2]) ? bounding_plane_d[Z_MID] : bounding_plane_d[Z_MIN],
          (child_bitset[0]) ? bounding_plane_d[X_MAX] : bounding_plane_d[X_MID],
          (child_bitset[1]) ? bounding_plane_d[Y_MAX] : bounding_plane_d[Y_MID],
          (child_bitset[2]) ? bounding_plane_d[Z_MAX] : bounding_plane_d[Z_MID], v);
      if (debug) std::cout << "is Point Node\n";
    }
    return;
  }
  // child is point node -> safe point that is already present, 
  //                        delete point node and make it a parent node, 
  //                        insert both points into parent node
  else {
    if (same_voxel_size) {
      if (debug) std::cout << "is already Point Node\n";
      return;
    }
    if (debug) std::cout << "was Point Node, ";
    Vertex v_present = children[child]->v;
    delete children[child];
    children[child] = nullptr;
    std::bitset<3> child_bitset(child);
    children[child] = new Octree(
        (child_bitset[0]) ? bounding_plane_d[X_MID] : bounding_plane_d[X_MIN],
        (child_bitset[1]) ? bounding_plane_d[Y_MID] : bounding_plane_d[Y_MIN],
        (child_bitset[2]) ? bounding_plane_d[Z_MID] : bounding_plane_d[Z_MIN],
        (child_bitset[0]) ? bounding_plane_d[X_MAX] : bounding_plane_d[X_MID],
        (child_bitset[1]) ? bounding_plane_d[Y_MAX] : bounding_plane_d[Y_MID],
        (child_bitset[2]) ? bounding_plane_d[Z_MAX] : bounding_plane_d[Z_MID], max_depth-1, same_voxel_size);
    if (debug) std::cout << "is Parent Node\n  Inserting Point 1\n";
    children[child]->insert_vertex(v_present, debug);
    if (debug) std::cout << "  Inserting Point 2\n";
    children[child]->insert_vertex(v, debug);
  }
}

void Octree::fill(std::string shape, int voxelcount, bool debug) {
  srand(time(0));
  if (shape == "cube") {
    float side_length = bounding_plane_d[X_MAX] - bounding_plane_d[X_MIN];
    for (int i=0; i<voxelcount; i++) {
      if (debug) std::cout << "Inserting Point\n";
      insert_vertex(
          Vertex(
            Vec3f(
              float(rand())/float((RAND_MAX))*side_length+bounding_plane_d[X_MIN], 
              float(rand())/float((RAND_MAX))*side_length+bounding_plane_d[Y_MIN], 
              float(rand())/float((RAND_MAX))*side_length+bounding_plane_d[Z_MIN]), 
            Material(standard, RGBi("random"), 1.2, 0.2)
            ), 
          debug);
    }
  } else if (shape == "sphere") {
    float half_side = bounding_plane_d[X_MAX] - bounding_plane_d[X_MID];
    for (int i=0; i<voxelcount; i++) {
      float angle1 = float(rand())/float((RAND_MAX)) * 2.*3.1415;
      float angle2 = float(rand())/float((RAND_MAX)) * 3.1415;
      if (debug) std::cout << "Inserting Point\n";
      insert_vertex(
          Vertex(
            Vec3f(
              half_side*cos(angle1)*sin(angle2)+bounding_plane_d[X_MID], 
              half_side*sin(angle1)*sin(angle2)+bounding_plane_d[Y_MID], 
              half_side*cos(angle2)+bounding_plane_d[Z_MID]),
            Material(standard, RGBi("random"), 1.2, 0.2)
            ), 
          debug);
    }
  } else if (shape == "cylinder") {
    float side_length = bounding_plane_d[X_MAX] - bounding_plane_d[X_MIN];
    for (int i=0; i<voxelcount; i++) {
      float angle = float(rand())/float((RAND_MAX)) * 2.*3.1415;
      float height = float(rand())/float((RAND_MAX)) * side_length;
      if (debug) std::cout << "Inserting Point\n";
      insert_vertex(
          Vertex(
            Vec3f(
              cos(angle)*(side_length/2.)+bounding_plane_d[X_MID], 
              sin(angle)*(side_length/2.)+bounding_plane_d[Y_MID], 
              height),
            Material(standard, RGBi("random"), 1.2, 0.2)
            ), 
          debug);
    }
  } else if (shape == "bunny") {
    // open bunny file
    std::ifstream file;
    file.open("models/bunny.txt");
    if (!file.is_open()) {
      std::cout << "error opening file\n";
    }
    // read file sperated by whitespace
    std::string coordinate;
    float coordinates[3];
    int i = 0;
    while(std::getline(file, coordinate, ' ')) {
      // coordinate string to float and adjust the size of the coordinates to fit inside 0 - 1
      coordinates[i] = (std::stof(coordinate)+100.)/180.;
      if (i == 2) {
        i = 0;
        insert_vertex(
            Vertex(
              Vec3f(coordinates[0], 1-coordinates[1], 1-coordinates[2]),
              Material(standard, RGBi("orange"), 1.2, 0.2)
              ), 
            debug);
      } else {
        i++;
      }
    }
  }
}

int Octree::count_voxels() {
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

// intersect a ray with the root node and changes inputted _t_min and _t_max if hit
bool Octree::ray_hit_node(Ray *ray, float *_t_min, float *_t_max) {
  // slab method
  float t_min = ray->min_t; 
  float t_max = ray->max_t; 

  float inverse_x = 1./ray->direction.x;
  float t_x0 = (bounding_plane_d[X_MIN] - ray->origin.x) * inverse_x;
  float t_x1 = (bounding_plane_d[X_MAX] - ray->origin.x) * inverse_x;
  if (ray->direction.x != 0.) {
    t_min = std::fmax(t_min, std::fmin(t_x0, t_x1));
    t_max = std::fmin(t_max, std::fmax(t_x0, t_x1));
  }
  float inverse_y = 1./ray->direction.y;
  float t_y0 = (bounding_plane_d[Y_MIN] - ray->origin.y) * inverse_y;
  float t_y1 = (bounding_plane_d[Y_MAX] - ray->origin.y) * inverse_y;
  if (ray->direction.y != 0.) {
    t_min = std::fmax(t_min, std::fmin(t_y0, t_y1));
    t_max = std::fmin(t_max, std::fmax(t_y0, t_y1));
  }
  float inverse_z = 1./ray->direction.z;
  float t_z0 = (bounding_plane_d[Z_MIN] - ray->origin.z) * inverse_z;
  float t_z1 = (bounding_plane_d[Z_MAX] - ray->origin.z) * inverse_z;
  if (ray->direction.z != 0.) {
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

// returns closest point node that the ray hit
bool Octree::intersection(Ray *ray, intersection_information *ii) {
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
          bool intersected = children[child_order[i]]->intersection(ray, ii);
          if (intersected) return true;
          // if nothing returned true, continue with next child
        }
        // child is point node -> return intersection information
        else if (children[child_order[i]]->type != Leaf) {
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
          ii->material = children[child_order[i]]->v.material;
          Vec3f cube_center(
              children[child_order[i]]->bounding_plane_d[X_MID], 
              children[child_order[i]]->bounding_plane_d[Y_MID], 
              children[child_order[i]]->bounding_plane_d[Z_MID]
              );
          Vec3f cti = (ii->point - cube_center).normalize(); // center to intersection (cti)
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
          return true;
        }
        // child is leaf node -> do nothing and continue with next child
      }
    }
  }
  return false;
}
