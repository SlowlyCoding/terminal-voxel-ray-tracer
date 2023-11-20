#include "../include/object.hpp"
#define OGT_VOX_IMPLEMENTATION
#include "../include_external/ogt_vox.h"

Object::Object(Vec3f _location, float _voxel_size, uint _voxel_count_x, uint _voxel_count_y, uint _voxel_count_z) {
    assert(_voxel_size > 0);
    assert(_voxel_count_x > 0); 
    assert(_voxel_count_y > 0);
    assert(_voxel_count_z > 0);
    voxel_size = _voxel_size;
    voxel_count_x = _voxel_count_x;
    voxel_count_y = _voxel_count_y;
    voxel_count_z = _voxel_count_z;
    bounds[0] = _location;
    bounds[1] = _location + Vec3f(voxel_count_x, voxel_count_y, voxel_count_z)*voxel_size;

    // fill the object with empty voxels
    voxels = std::vector<uint8_t>(voxel_count_x*voxel_count_y*voxel_count_z,255);
}

Object::Object(Config& config) {
    // open the file
    FILE * fp = fopen(config.object_model_path.c_str(), "rb");
    if (!fp) return;

    // get the buffer size which matches the size of the file
    fseek(fp, 0, SEEK_END);
    uint32_t buffer_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // load the file into a memory buffer
    uint8_t * buffer = new uint8_t[buffer_size];
    fread(buffer, buffer_size, 1, fp);
    fclose(fp);

    // construct the scene from the buffer
    const ogt_vox_scene * scene = ogt_vox_read_scene_with_flags(buffer, buffer_size, 0);

    // the buffer can be safely deleted once the scene is instantiated.
    delete[] buffer;
    if (!scene) {
        printf("couldnt open vox file\n");
        return;
    }

    voxel_size = config.object_voxel_size;
    voxel_count_x = scene->models[0]->size_x; 
    voxel_count_y = scene->models[0]->size_y; 
    voxel_count_z = scene->models[0]->size_z; 
    bounds[0] = config.object_location;
    bounds[1] = config.object_location + Vec3f(voxel_count_x, voxel_count_y, voxel_count_z)*voxel_size;
    voxels = std::vector<uint8_t>(voxel_count_x*voxel_count_y*voxel_count_z,255);
    long int voxel_count = 0;
    for(int i=0; i<voxel_count_x*voxel_count_y*voxel_count_z; i++) {
        if (scene->models[0]->voxel_data[i] != 0) {
            change_voxel_material(i, config.object_color_index);
            voxel_count++;
        }

    }
    std::cout << "\nVoxels: " << voxel_count << std::endl;
    ogt_vox_destroy_scene(scene);
}

void Object::change_material_at_palette_index(uint8_t index, Material material) {
    assert(index > 0);
    assert(index < 255);
    palette[index] = material;
}

void Object::resize_x(uint new_voxel_count_x) {
    // TODO: this
    if (voxel_count_x < new_voxel_count_x) {
        for (int y=0; y<voxel_count_y; y++) {
            for (int z=0; z<voxel_count_z; z++) {
                for (int x=0; x<new_voxel_count_x-voxel_count_x; x++) {
                    voxels.insert(
                            voxels.begin() + 
                            z*voxel_count_x*voxel_count_y + 
                            y*voxel_count_x + 
                            x + voxel_count_x, 255);
                }
            }
        }
    }
    voxel_count_x = new_voxel_count_x;
}

void Object::move_to(Vec3f new_location) {
    Vec3f d = new_location - bounds[0];
    bounds[0] = new_location;
    bounds[1] = bounds[1] + d;
}

void Object::change_voxel_material(uint voxel_index, uint8_t material_index) {
    assert(material_index < 255); // 255 is reserved for air
    assert(voxel_index >= 0);
    assert(voxel_index < voxel_count_x*voxel_count_y*voxel_count_z);
    voxels[voxel_index] = material_index;
}

bool Object::intersect_bounding_box(Ray const& ray, float *tmin, float *tmax) {
    // check if ray.origin is inside box
    if (ray.origin.x >= bounds[0].x &&
            ray.origin.y >= bounds[0].y &&
            ray.origin.z >= bounds[0].z &&
            ray.origin.x <= bounds[1].x &&
            ray.origin.y <= bounds[1].y &&
            ray.origin.z <= bounds[1].z
       ) {
        *tmin = 0;
        return true;
    }
    // An Efficient and Robust Ray–Box Intersection Algorithm
    // https://people.csail.mit.edu/amy/papers/box-jgt.pdf
    float txmin, txmax, tymin, tymax, tzmin, tzmax;
    *tmin = (bounds[ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
    *tmax = (bounds[1-ray.sign[0]].x - ray.origin.x) * ray.inv_direction.x;
    tymin = (bounds[ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
    tymax = (bounds[1-ray.sign[1]].y - ray.origin.y) * ray.inv_direction.y;
    if ((*tmin > tymax) || (tymin > *tmax)) return false;
    if (tymin > *tmin) *tmin = tymin;
    if (tymax < *tmax) *tmax = tymax;
    tzmin = (bounds[ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
    tzmax = (bounds[1-ray.sign[2]].z - ray.origin.z) * ray.inv_direction.z;
    if ((*tmin > tzmax) || (tzmin > *tmax)) return false;
    if (tzmin > *tmin) *tmin = tzmin;
    if (tzmax < *tmax) *tmax = tzmax;
    return ((*tmax < ray.max_t) && (*tmin > ray.min_t));
}

std::tuple<int,int,int> Object::find_starting_voxel(Vec3f const& object_hit_point) {
    Vec3f d = (object_hit_point - bounds[0]);
    assert(d.x >= -0.001 && d.y >= -0.001 && d.z >= -0.001);
    int x = d.x/voxel_size;
    if (x == voxel_count_x) x--;
    int y = d.y/voxel_size;
    if (y == voxel_count_y) y--;
    int z = d.z/voxel_size;
    if (z == voxel_count_z) z--;
    return std::make_tuple(x,y,z);
}

bool Object::intersect(Ray const& ray, intersection_information *ii, bool debug) {
    // first intersect the ray with the bounding volume
    float tmin,tmax;
    if (!intersect_bounding_box(ray, &tmin, &tmax)) return false;

    Ray box_ray = ray;
    if (tmin == 0) {
        if (debug) std::cout << "Ray starts inside object" << std::endl;
    }
    else {
        box_ray.origin = ray.point(tmin);
        if (debug) {
            std::cout << "Ray enters object at " << box_ray.origin << std::endl;
        }
    }
    if (debug) {
        Vec3f exit = ray.point(tmax);
        std::cout << " Ray exits object at " << exit << std::endl;
    }

    // initialization: find starting voxel
    int x,y,z;
    std::tie(x,y,z) = find_starting_voxel(box_ray.origin);
    if (debug) printf("Ray enters object at voxel: X%i Y%i Z%i\n",x,y,z);
    if (debug) printf("Stepping direction (%i, %i, %i)\n",ray.stepx,ray.stepy,ray.stepz);

    // init variables for incremental traversal
    Vec3f what = bounds[0] - box_ray.origin;
    if (ray.stepx == 1) what.x += (x+1)*voxel_size;
    else what.x += x*voxel_size;
    if (ray.stepy == 1) what.y += (y+1)*voxel_size;
    else what.y += y*voxel_size;
    if (ray.stepz == 1) what.z += (z+1)*voxel_size;
    else what.z += z*voxel_size;
    float txmax = std::abs(what.x * box_ray.inv_direction.x);
    float tymax = std::abs(what.y * box_ray.inv_direction.y);
    float tzmax = std::abs(what.z * box_ray.inv_direction.z);
    if (debug) printf("tmax's x%f y%f z%f\n", txmax,tymax,tzmax);
    Vec3f what2(voxel_size, voxel_size, voxel_size);
    float txdelta = std::abs(what2.x * box_ray.inv_direction.x);
    float tydelta = std::abs(what2.y * box_ray.inv_direction.y);
    float tzdelta = std::abs(what2.z * box_ray.inv_direction.z);
    if (debug) printf("tdelta's x%f y%f z%f\n", txdelta,tydelta,tzdelta);

    // incremental traversal: ray march until a solid voxel is hit
    // A Fast Voxel Traversal Algorithm for Ray Tracing
    // http://www.cse.yorku.ca/~amana/research/grid.pdf
    uint8_t material_index = voxels[z*voxel_count_x*voxel_count_y + y*voxel_count_x + x];
    while (material_index == 255) {
        if (txmax < tymax) {
            if(txmax < tzmax) {
                x = x + ray.stepx;
                if(x == voxel_count_x || x == -1) return false;
                txmax = txmax + txdelta;
            } else {
                z = z + ray.stepz;
                if(z == voxel_count_z || z == -1) return false;
                tzmax = tzmax + tzdelta;
            }
        } else {
            if(tymax < tzmax) {
                y = y + ray.stepy;
                if(y == voxel_count_y || y == -1) return false;
                tymax = tymax + tydelta;
            } else {
                z = z + ray.stepz;
                if(z == voxel_count_z || z == -1) return false;
                tzmax = tzmax + tzdelta;
            }
        }
        if (debug) printf("  looking at voxel: X%i Y%i Z%i\n",x,y,z);
        material_index = voxels[z*voxel_count_x*voxel_count_y + y*voxel_count_x + x];
    }
    if (debug) printf("Voxel hit: X%i Y%i Z%i\n",x,y,z);
    if (debug) std::cout << "Matieral index: " << (int)material_index << std::endl;

    txmax -= txdelta;
    tymax -= tydelta;
    tzmax -= tzdelta;
    float smallest_t_axis;
    if (txmax > tymax) {
        if (txmax > tzmax) {
            smallest_t_axis = txmax;
            ii->normal = Vec3f(-ray.stepx,0,0);
        } else {
            smallest_t_axis = tzmax;
            ii->normal = Vec3f(0,0,-ray.stepz);
        }
    } else {
        if (tymax > tzmax) {
            smallest_t_axis = tymax;
            ii->normal = Vec3f(0,-ray.stepy,0);
        } else {
            smallest_t_axis = tzmax;
            ii->normal = Vec3f(0,0,-ray.stepz);
        }
    }
    ii->t = smallest_t_axis;
    ii->point = box_ray.point(smallest_t_axis);
    ii->material = &palette[material_index];
    return true;
}
