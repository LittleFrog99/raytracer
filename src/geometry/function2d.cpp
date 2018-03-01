#include "function2d.h"

bool Function2D::intersect(const Ray &ray, double &tmin, Shade &sr) const {
    tmin = 100;
    sr.color = function(ray.origin.x, ray.origin.y);
    sr.hitPoint = vec4(ray.origin.x, ray.origin.y, 100, 1);
    sr.normal = vec3(0, 0, 1);
    return true;
}
