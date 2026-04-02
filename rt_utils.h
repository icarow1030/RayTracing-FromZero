#ifndef RT_UTILS_H
#define RT_UTILS_H

#include "vec3.h"
#include "ray.h"

const double infinity = std::numeric_limits<double>::infinity();

void solve_sphere_quadratic(const ray& r, double& a, double& h, double& c, const point3& center, double radius) {
    vec3 oc = center - r.origin();
    a = r.direction().length_squared();
    h = dot(r.direction(), oc);
    c = oc.length_squared() - radius*radius;
}

#endif //RT_UTILS_H