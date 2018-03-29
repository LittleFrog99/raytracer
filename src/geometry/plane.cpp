#include "plane.h"

void Plane::setParams(dvec3 normal, dvec3 point) {
    double d = -dot(normal, point);
    param = dvec4(normal, d);
}

bool Plane::intersect(Ray &ray, double &tmin, Shade &shade) {
    double t = -dot(param, dvec4(ray.origin, 1.0)) / dot(param, dvec4(ray.direction, 0.0));
    if (t > EPSILON) {
        tmin = t;
        shade.normal = vec3(param);
        shade.localHitPoint = ray.origin + t * ray.direction;
        shade.hitPoint = shade.localHitPoint;
        return true;
    }
    else return false;
}

bool Plane::shadowIntersect(Ray &ray, double &tmin) {
    double t = -dot(param, dvec4(ray.origin, 1.0)) / dot(param, dvec4(ray.direction, 0.0));
    if (t > EPSILON) {
        tmin = t;
        return true;
    }
    else return false;
}