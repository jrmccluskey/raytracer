#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
public:
  sphere() {}
  sphere(vec3 cen, double r) : center(cen), radius(r) {};
  virtual bool hit(const ray& r, double tMin, double tMax, hit_record& rec) const;
  vec3 center;
  double radius;
};

bool sphere::hit(const ray& r, double tMin, double tMax, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  double a = dot(r.direction(), r.direction());
  double b = dot(oc, r.direction());
  double c = dot(oc, oc) - radius*radius;
  double discriminant = b*b - a*c;
  if(discriminant > 0) {
    double temp = (-b - sqrt(b*b - a*c))/a;
    if(temp < tMax && temp > tMin) {
      rec.t = temp;
      rec.p = r.pointAtParameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      return true;
    }
    temp = (-b + sqrt(b*b - a*c))/a;
    if(temp < tMax && temp > tMin) {
      rec.t = temp;
      rec.p = r.pointAtParameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      return true;
    }
  }
  return false;
}

#endif
