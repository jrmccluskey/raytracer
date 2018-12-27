#include <iostream>
#include "ray.h"

bool hitSphere(const vec3& center, double radius, const ray& r) {
  vec3 oc = r.origin() - center;
  double a = dot(r.direction(), r.direction());
  double b = 2.0 * dot(oc, r.direction());
  double c = dot(oc, oc) - radius*radius;
  double discriminant = b*b - -4*a*c;
  return (discriminant > 0);
}

vec3 color(const ray& r) {
  if(hitSphere(vec3(0,0,-1), 0.5, r)) {
    return vec3 (1,0,0);
  }
  vec3 unitDirection = unitVector(r.direction());
  double t = 0.5*(unitDirection.y() + 1.0);
  return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

int main() {
  int nx = 200;
  int ny = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  vec3 lowerLeftCorner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  vec3 origin(0.0, 0.0, 0.0);
  for(int j = ny-1; j >= 0; j--) {
    for(int i = 0; i < nx; i++) {
      double u = double(i) / double(nx);
      double v = double(j) / double(ny);
      ray r(origin, lowerLeftCorner + u*horizontal + v*vertical);
      vec3 col = color(r);

      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);

      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}
