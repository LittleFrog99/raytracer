#include "partsphere.h"

void PartSphere::setParams(dvec3 center, double radius, double theta_min,
                           double theta_max, double phi_min, double phi_max) {
    Sphere::setParams(center, radius);
    this->thetaMax = theta_max;
    this->cosThetaMax = cos(theta_max);
    this->cosThetaMin = cos(theta_min);
    this->thetaMin = theta_min;
    this->phiMax = phi_max;
    this->phiMin = phi_min;
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

inline bool PartSphere::inRange(dvec3 hitDir) {
    double phi = atan2(hitDir.x, hitDir.z);
    if (phi < 0.0) phi += 2 * PI;
    return hitDir.y < radius * cosThetaMin && hitDir.y > radius * cosThetaMax
           && phi > phiMin && phi < phiMax;
}