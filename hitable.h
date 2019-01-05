#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class material;

struct hit_record {
  double t;
  vec3 p;
  vec3 normal;
  material *matPtr;
};

class hitable {
public:
  virtual bool hit(const ray& r, double tMin, double tMax, hit_record& rec) const = 0;
};

#endif
