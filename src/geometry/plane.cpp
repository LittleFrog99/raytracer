#include "plane.h"

Plane::Plane(dvec3 normal, dvec3 point, Material *material_ptr) : 
    Geometry(material_ptr) {
    double d = -dot(normal, point);
    param = dvec4(normal, d);
}

bool Plane::intersect(Ray &ray, double &tmin, Shade &shade) {
    double t = -dot(param, dvec4(ray.origin, 1.0)) / dot(param, dvec4(ray.direction, 0.0));
    if (t > EPSILON) {
        tmin = t;
        shade.normal = vec3(param);
        shade.hitPoint = ray.origin + t * ray.direction;
        return true;
    }
    else return false;
}
