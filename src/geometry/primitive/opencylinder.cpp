#include "opencylinder.h"

void OpenCylinder::setParams(double bottom_pos, double top_pos, double radius) {
    if (bottom_pos > top_pos) swap(bottom_pos, top_pos);
    bottom = bottom_pos;
    top = top_pos;
    this->radius = radius;
}

dvec3 OpenCylinder::getNormal(dvec3 &point) {
    return normalize(dvec3(point.x, 0.0, point.z));
}

bool OpenCylinder::intersect(Ray &ray, double &tmin, Shade &shade) {
    double a = pow(ray.direction.x, 2) + pow(ray.direction.z, 2);
    double b = 2.0 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z);
    double c = pow(ray.origin.x, 2) + pow(ray.origin.z, 2) - radius * radius;
    double discr = b * b - 4.0 * a * c;

    if (discr < 0.0) return false;
    double e = sqrt(discr);
    double denom = 2.0 * a;

    double t = (-b - e) / denom;
    if (t > EPSILON) {
        dvec3 hitPt = ray.origin + t * ray.direction;
        if (hitPt.y > bottom && hitPt.y < top) {
            tmin = t;
            shade.normal = getNormal(hitPt);
            if (dot(-ray.direction, shade.normal) < 0.0)
                shade.normal = -shade.normal;
            shade.localHitPoint = hitPt;
            return true;
        }
    }

    t = (-b + e) / denom;
    if (t > EPSILON) {
        dvec3 hitPt = ray.origin + t * ray.direction;
        if (hitPt.y > bottom && hitPt.y < top) {
            tmin = t;
            shade.normal = getNormal(hitPt);
            if (dot(-ray.direction, shade.normal) < 0.0)
                shade.normal = -shade.normal;
            shade.localHitPoint = hitPt;
            return true;
        }
    }

    return false;
}

bool OpenCylinder::shadowIntersect(Ray &ray, double &tmin) {
    double a = pow(ray.direction.x, 2) + pow(ray.direction.z, 2);
    double b = 2.0 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z);
    double c = pow(ray.origin.x, 2) + pow(ray.origin.z, 2) - radius * radius;
    double discr = b * b - 4.0 * a * c;

    if (discr < 0.0) return false;
    double e = sqrt(discr);
    double denom = 2.0 * a;

    double t = (-b - e) / denom;
    if (t > EPSILON) {
        dvec3 hitPt = ray.origin + t * ray.direction;
        if (hitPt.y > bottom && hitPt.y < top) {
            tmin = t;
            return true;
        }
    }

    t = (-b + e) / denom;
    if (t > EPSILON) {
        dvec3 hitPt = ray.origin + t * ray.direction;
        if (hitPt.y > bottom && hitPt.y < top) {
            tmin = t;
            return true;
        }
    }
    return false;
}
