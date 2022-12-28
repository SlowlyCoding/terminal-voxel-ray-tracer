#pragma once
#include "vector.hpp"
#include "material.hpp"

/* class Vertex { */
/*   public: */
/*     Vec3f point; */
/*     Material *material; */
/*     Vertex() : point(Vec3f()), material(nullptr) {}; */
/*     Vertex(Vec3f point, Material *material) : point(point), material(material) {}; */
/* }; */

struct Vertex {
  Vec3f point;
  Material *material;
  Vertex() : point(Vec3f()), material(nullptr) {};
  Vertex(Vec3f point, Material *material) : point(point), material(material) {};
};
