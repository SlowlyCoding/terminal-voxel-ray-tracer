#pragma once
#include "vector.hpp"
#include "material.hpp"

class Vertex {
  public:
    Vec3f point;
    Material material;
    Vertex() : point(Vec3f()), material(Material()) {};
    Vertex(Vec3f point, Material material) : point(point), material(material) {};
};
