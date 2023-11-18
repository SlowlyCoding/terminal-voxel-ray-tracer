#pragma once
#include "vector.hpp"
#include "material.hpp"

struct intersection_information {
    float t;
    Vec3f point; 
    Vec3f normal;
    Material *material;
    bool inside_voxel;

    void debug() {
        std::cout << "t: " << t << std::endl;
        std::cout << "point: ";
        point.values();
        std::cout << "normal: ";
        normal.values();
        std::cout << "material color: " << material->color.r << " " << material->color.g << " " << material->color.b << std::endl;;
        inside_voxel ? std::cout << "inside voxel true\n" : std::cout << "inside voxel false\n";

    }
};


