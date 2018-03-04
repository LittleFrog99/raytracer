#include "plane.h"

Plane::Plane(dvec3 normal, dvec3 point, vec3 color) : Geometry(color) {
    double d = -dot(normal, point);
    param = dvec4(normal, d);
}

bool Plane::intersect(const Ray &ray, double &tmin, Shade &shade) const {
    double t = -dot(param, ray.origin) / dot(param, ray.direction);
    if (t > EPSILON) {
        tmin = t;
        shade.normal = vec3(param);
        shade.hitPoint = ray.origin + t * ray.direction;
        return true;
    }
    else return false;
}
