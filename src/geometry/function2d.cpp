#include "function2d.h"

bool Function2D::intersect(Ray &ray, double &tmin, Shade &sr) {
    tmin = 100;
    sr.color = function(ray.origin.x, ray.origin.y);
    sr.hitPoint = vec3(ray.origin.x, ray.origin.y, 100);
    sr.normal = vec3(0, 0, 1);
    return true;
}
