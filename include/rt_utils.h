#ifndef RT_UTILS_H
#define RT_UTILS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include <random>


const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

void solve_sphere_quadratic(const ray& r, double& a, double& h, double& c, const point3& center, double radius) {
    vec3 oc = center - r.origin();
    a = r.direction().length_squared();
    h = dot(r.direction(), oc);
    c = oc.length_squared() - radius*radius;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

#endif //RT_UTILS_H