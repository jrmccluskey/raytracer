#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "ray.h"
#include "hitable.h"

vec3 randomInUnitSphere() {
  vec3 p;
  do {
    p = 2.0 * vec3(drand48(),drand48(), drand48()) - vec3(1,1,1);
  } while(p.squaredLength() >= 1.0);
  return p;
}

vec3 reflect(const vec3 & v, const vec3& n) {
  return v - 2*dot(v,n)*n;
}

bool refract(const vec3& v, const vec3& n, double niOverNt, vec3& refracted) {
  vec3 uv = unitVector(v);
  double dt = dot(uv, n);
  double discriminant = 1.0 - niOverNt*niOverNt*(1-dt*dt);
  if(discriminant > 0) {
    refracted = niOverNt*(uv - n*dt) - n*sqrt(discriminant);
    return true;
  } else {
    return false;
  }
}

double schlick(double cosine, double refIdx) {
  double r0 = (1-refIdx) / (1+refIdx);
  r0 = r0 * r0;
  return r0 + (1-r0)*pow((1- cosine), 5);
}

class material {
public:
  virtual bool scatter(const ray& rIn, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
  lambertian(const vec3& a) : albedo(a) {}
  virtual bool scatter(const ray& rIn, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 target = rec.p + rec.normal + randomInUnitSphere();
    scattered = ray(rec.p, target-rec.p);
    attenuation = albedo;
    return true;
  }

  vec3 albedo;
};

class metal : public material {
public:
  metal(const vec3& a, double f) : albedo(a) {if(f < 1) fuzz = f; else fuzz = 1;}
  virtual bool scatter(const ray& rIn, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 reflected = reflect(unitVector(rIn.direction()), rec.normal);
    scattered = ray(rec.p, reflected+fuzz*randomInUnitSphere());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }

  vec3 albedo;
  double fuzz;
};

class dielectric : public material {
public:
  dielectric(double ri) : refIdx(ri) {}
  virtual bool scatter(const ray& rIn, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 outwardNormal;
    vec3 reflected = reflect(rIn.direction(), rec.normal);
    double niOverNt;
    attenuation = vec3(1.0, 1.0, 1.0);
    vec3 refracted;
    double reflectProb;
    double cosine;

    if(dot(rIn.direction(), rec.normal) > 0) {
      outwardNormal = -rec.normal;
      niOverNt = refIdx;
      cosine = refIdx * dot(rIn.direction(), rec.normal) / rIn.direction().length();
    } else {
      outwardNormal = rec.normal;
      niOverNt = 1.0 / refIdx;
      cosine = -dot(rIn.direction(), rec.normal) / rIn.direction().length();
    }

    if(refract(rIn.direction(), outwardNormal, niOverNt, refracted)) {
      reflectProb = schlick(cosine, refIdx);
    } else {
      scattered = ray(rec.p, reflected);
      reflectProb = 1.0;
    }
    if(drand48() < reflectProb) {
      scattered = ray(rec.p, reflected);
    } else {
      scattered = ray(rec.p, refracted);
    }
    return true;
  }

  double refIdx;
};

#endif
