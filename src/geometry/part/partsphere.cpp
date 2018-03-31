#include "partsphere.h"

void PartSphere::setParams(dvec3 center, double radius, double azim_min,
                           double azim_max, double polar_min, double polar_max) {
    Sphere::setParams(center, radius);
    this->azimMin = azim_min;
    this->azimMax = azim_max;
    this->polarMin = polar_min;
    this->polarMax = polar_max;
    this->cosPolarMin = cos(polar_min);
    this->cosPolarMax = cos(polar_max);
}

inline bool PartSphere::inRange(dvec3 &hitDir) {
    double azim = atan2(hitDir.x, hitDir.z);
    if (azim < 0.0)
        azim += 2 * PI;
    return hitDir.y < (radius * cosPolarMin) && hitDir.y > (radius * cosPolarMax) 
           && azim > azimMin && azim < azimMax;
}

bool PartSphere::intersect(Ray &ray, double &tmin, Shade &shade) {
    double t;
    dvec3 dist = ray.origin - center;
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(dist, ray.direction);
    double c = dot(dist, dist) - radius * radius;
    double disc = b * b - 4.0 * a * c;

    if (disc < 0) return false;

    double e = sqrt(disc);
    double denom = 2.0 * a;
    t = (-b - e) / denom; // smaller root
    if (t > EPSILON) {
        dvec3 hitPt = ray.origin + t * ray.direction;
        dvec3 hitDir = hitPt - center;

        if (inRange(hitDir)) {
            tmin = t;
            shade.normal = normalize(hitDir);
            shade.localHitPoint = hitPt;
            shade.hitPoint = shade.localHitPoint;
            return true;
        }
    }

    t = (-b + e) / denom;
    if (t > EPSILON) {
        dvec3 hitPt = ray.origin + t * ray.direction;
        dvec3 hitDir = hitPt - center;

        if (inRange(hitDir)) {
            tmin = t;
            shade.normal = normalize(hitDir);
            if (dot(-ray.direction, shade.normal) < 0.0)
                shade.normal = -shade.normal;
            shade.localHitPoint = hitPt;
            shade.hitPoint = shade.localHitPoint;
            return true;
        }
    }

    return false;
}

bool PartSphere::shadowIntersect(Ray &ray, double &tmin) {
    double t;
    dvec3 dist = ray.origin - center;
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(dist, ray.direction);
    double c = dot(dist, dist) - radius * radius;
    double disc = b * b - 4.0 * a * c;

    if (disc < 0) return false;

    double e = sqrt(disc);
    double denom = 2.0 * a;
    t = (-b - e) / denom; // smaller root
    if (t > EPSILON) {
        dvec3 hitPt = ray.origin + t * ray.direction;
        dvec3 hitDir = hitPt - center;

        if (inRange(hitDir)) {
            tmin = t;
            return true;
        }
    }

    t = (-b + e) / denom;
    if (t > EPSILON) {
        dvec3 hitPt = ray.origin + t * ray.direction;
        dvec3 hitDir = hitPt - center;

        if (inRange(hitDir)) {
            tmin = t;
            return true;
        }
    }

    return false;
}
