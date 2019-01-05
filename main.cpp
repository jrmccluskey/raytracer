#include <iostream>
#include <random>
#include "ray.h"
#include "sphere.h"
#include "hitableList.h"
#include "float.h"
#include "camera.h"
#include "material.h"

vec3 color(const ray& r, hitable *world, int depth) {
  hit_record rec;
  if(world->hit(r, 0.001, DBL_MAX, rec)) {
    ray scattered;
    vec3 attenuation;
    if(depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * color(scattered, world, depth+1);
    } else {
      return vec3(0,0,0);
    }
  } else {
    vec3 unitDirection = unitVector(r.direction());
    double t = 0.5*(unitDirection.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
  }
}

hitable *randomScene() {
  int n = 500;
  hitable **list = new hitable*[n+1];
  list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
  int i = 1;
  for(int a = -11; a < 11; a++) {
    for(int b = -11; b < 11; b++) {
      double chooseMat = drand48();
      vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
      if((center - vec3(4, 0.2, 0)).length() > 0.9) {
        if(chooseMat < 0.8) {
          list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
        } else if(chooseMat < 0.75) {
          list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1+drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48()));
        } else {
          list[i++] = new sphere(center, 0.2, new dielectric(1.5));
        }
      }
    }
  }

  list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
  list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
  list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

  return new hitableList(list, i);
}

int main() {
  int nx = 1200;
  int ny = 800;
  int ns = 10;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  hitable *world = randomScene();

  vec3 lookFrom(13, 2, 3);
  vec3 lookAt(0, 0, 0);
  double distToFocus = 10.0;
  double aperature = 0.1;

  camera cam(lookFrom, lookAt, vec3(0, 1, 0), 20, double(nx)/double(ny), aperature, distToFocus);
  for(int j = ny-1; j >= 0; j--) {
    for(int i = 0; i < nx; i++) {
      vec3 col(0,0,0);
      for(int k = 0; k < ns; k++) {
        double u = double(i + drand48()) / double(nx);
        double v = double(j + drand48()) / double(ny);
        ray r = cam.getRay(u, v);
        vec3 p = r.pointAtParameter(2.0);
        col += color(r, world,0);
      }

      col /= double(ns);
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);

      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}
