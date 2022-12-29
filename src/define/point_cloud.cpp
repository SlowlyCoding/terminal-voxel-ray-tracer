#include "../include/point_cloud.hpp"

void create_point_cloud(
    std::vector<Vertex> *vertices, 
    Shape shape, 
    int point_count, 
    Material *material, 
    Octree *root
) {
  switch (shape) {
    case grid: {
      int half_side_length_in_voxels = cbrt(pow(8,root->current_height))*0.5f;
      for (int z=1; z<=half_side_length_in_voxels; z++) {
        for (int y=1; y<=half_side_length_in_voxels; y++) {
          for (int x=1; x<=half_side_length_in_voxels; x++) {
            vertices->push_back(
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
        vertices->push_back(
            Vertex(
              Vec3f(
                root->radius*cos(angle1)*sin(angle2)+root->center.x, 
                root->radius*sin(angle1)*sin(angle2)+root->center.y, 
                root->radius*cos(angle2)+root->center.z),
              material
              ));
      }
      break;
                 }
    case cylinder: {
      for (int i=0; i<point_count; i++) {
        float angle = float(rand())/float((RAND_MAX)) * 2.*3.1415;
        float height = float(rand())/float((RAND_MAX)) * root->radius*2.0f;
        vertices->push_back(
            Vertex(
              Vec3f(
                cos(angle)*root->radius+root->center.x, 
                sin(angle)*root->radius+root->center.y, 
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
        float side_length_in_voxels = cbrt(pow(8, root->current_height));
        for (int y=0; y<side_length_in_voxels; y++) {
          for (int x=0; x<side_length_in_voxels; x++) {
            noise = perlin.octave2D_01(x/side_length_in_voxels, y/side_length_in_voxels, 5);
            while (noise > 0) {
              vertices->push_back(Vertex(Vec3f(x/side_length_in_voxels, y/side_length_in_voxels, noise), material));
              noise -= 1.0f/side_length_in_voxels;
            }
          }
        }
        break;
                  }
  }
}
