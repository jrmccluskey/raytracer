#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
public:
  camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, double vfov, double aspect) {
    vec3 u, v, w;
    double theta = vfov*M_PI/180;
    double halfHeight = tan(theta/2);
    double halfWidth = aspect*halfHeight;
    origin = lookFrom;
    w = unitVector(lookFrom - lookAt);
    u = unitVector(cross(vUp, w));
    v = cross(w, u);
    lowerLeftCorner = origin - halfWidth*u - halfHeight*v - w;
    horizontal = 2*halfWidth*u;
    vertical = 2*halfHeight*v;
  }

  ray getRay(double u, double v) {return ray(origin, lowerLeftCorner+ u*horizontal + v*vertical - origin);}

  vec3 origin;
  vec3 lowerLeftCorner;
  vec3 horizontal;
  vec3 vertical;
};

#endif
