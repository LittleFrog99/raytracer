#include "rectangle.h"
#include "core/sampler.h"

void Rectangle::setParams(dvec3 origin, dvec3 side_a, dvec3 side_b) {
    this->origin = origin;
    sideA = side_a;
    lenASqrd = Math::lengthSquared(sideA);
    sideB = side_b;
    lenBSqrd = Math::lengthSquared(sideB);
    normal = normalize(cross(sideA, sideB));
    area = length(sideA) * length(sideB);
    invArea = 1 / area;
}

dvec3 Rectangle::sample() {
    dvec2 samplePt = samplerP->sampleUnitSquare();
    return (origin + samplePt.x * sideA + samplePt.y * sideB);
}

bool Rectangle::intersect(Ray &ray, double &tmin, Shade &shade) {
    double t = dot(origin - ray.origin, normal) / dot(ray.direction, normal);
    if (t <= EPSILON) return false;

    dvec3 hitPt = ray.origin + t * ray.direction;
    dvec3 dir = hitPt - origin;

    double dDotA = dot(dir, sideA);
    if (dDotA < 0.0 || dDotA > lenASqrd) return false;
    double dDotB = dot(dir, sideB);
    if (dDotB < 0.0 || dDotB > lenBSqrd) return false;

    tmin = t;
    shade.normal = normal;
    shade.localHitPoint = hitPt;
    shade.hitPoint = shade.localHitPoint;
    shade.materialP = getMaterial();
    return true;
}

bool Rectangle::shadowIntersect(Ray &ray, double &tmin) {
    double t = dot(origin - ray.origin, normal) / dot(ray.direction, normal);
    if (t <= EPSILON) return false;

    dvec3 hitPt = ray.origin + t * ray.direction;
    dvec3 dir = hitPt - origin;

    double dDotA = dot(dir, sideA);
    if (dDotA < 0.0 || dDotA > lenASqrd) return false;
    double dDotB = dot(dir, sideB);
    if (dDotB < 0.0 || dDotB > lenBSqrd) return false;

    tmin = t;
    return true;
}
