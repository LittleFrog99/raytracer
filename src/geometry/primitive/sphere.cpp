#include "sphere.h"

bool Sphere::intersect(Ray &ray, double &tmin, Shade &shade) {
    double t;
    dvec3 omc = ray.origin - center;
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(omc, ray.direction);
    double c = dot(omc, omc) - radius * radius;
    double disc = b * b - 4.0 * a * c;

    if (disc < 0) return false;
    else {
        double e = sqrt(disc);
        double denom = 2.0 * a;
        t = (-b - e) / denom; // smaller root
        if (t > EPSILON) {
            tmin = t;
            shade.normal = (omc + t * dvec3(ray.direction)) / radius;
            shade.hitPoint = ray.origin + t * ray.direction;
            return true;
        }
        t = (-b + e) / denom;
        if (t > EPSILON) {
            tmin = t;
            shade.normal = (omc + t * dvec3(ray.direction)) / radius;
            shade.localHitPoint = ray.origin + t * ray.direction;
            shade.hitPoint = shade.localHitPoint;
            return true;
        }
    }
    return false;
}

bool Sphere::shadowIntersect(Ray &ray, double &tmin) {
    double t;
    dvec3 omc = ray.origin - center;
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(omc, ray.direction);
    double c = dot(omc, omc) - radius * radius;
    double disc = b * b - 4.0 * a * c;

    if (disc < 0)
        return false;
    else {
        double e = sqrt(disc);
        double denom = 2.0 * a;
        t = (-b - e) / denom; // smaller root
        if (t > EPSILON) {
            tmin = t;
            return true;
        }
        t = (-b + e) / denom;
        if (t > EPSILON) {
            tmin = t;
            return true;
        }
    }
    return false;
}