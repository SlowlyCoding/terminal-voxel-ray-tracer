#pragma once
#include <cassert>
#include <vector>
#include <iomanip>
#include <tuple>
#include "config.hpp"
#include "vector.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "intersection_information.hpp"

class Object {
private:
    bool intersect_bounding_box(Ray const& ray, float *tmin, float *tmax);
    std::tuple<int,int,int> find_starting_voxel(Vec3f const& object_hit_point);
public:
    /* 
        the material palette can store up to 255 materials,
        this way they can be accesed by a single byte index,
        index 255 (last item) is reserved for empty voxels (air)
    */ 
    Material palette[255];

    /* 
        dimensions and location
    */ 
    float voxel_size;
    uint voxel_count_x, voxel_count_y, voxel_count_z;
    Vec3f bounds[2]; // 2 vectors defining the bounding box of the voxel volume

    /* 
        each voxel is stored inside the voxels array.
        only the index of the material each voxel is using is stored 
        in this flattened 3D array
    */ 
    std::vector<uint8_t> voxels;

    Object(Config& config);
    Object(Vec3f _location, float _voxel_size, uint voxel_count_x, uint voxel_count_y, uint voxel_count_z);

    /* palette managment */
    void change_material_at_palette_index(uint8_t index, Material material);
    /* dimensions and location managment */
    void resize_x(uint new_voxel_count_x);
    void move_to(Vec3f new_location);
    /* voxel managment */
    void change_voxel_material(uint voxel_index, uint8_t material_index);

    /* 
        intersect the object with a ray.
        the intersection information of the first voxel hit will be
        safed inside ii.
        if the ray hits no voxels, false is returned
    */
    bool intersect(Ray const& ray, intersection_information *ii, bool debug);
};
