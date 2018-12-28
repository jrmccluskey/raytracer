#include "ray.h"

class material {
public:
  virtual bool scatter(const ray& rIn, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};
