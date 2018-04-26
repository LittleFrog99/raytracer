#include "disk.h"

void Disk::setParams(dvec3 center, dvec3 normal, double radius) {
    this->center = center;
    this->normal = normalize(normal);
    this->radius = radius;
    this->radiusSquared = radius * radius;
    this->area = PI * radiusSquared;
}

bool Disk::intersect(Ray &ray, double &tmin, Shade &shade) {
    double t = dot(center - ray.origin, normal) / dot(ray.direction, normal);
    if (t <= EPSILON) return false;

    dvec3 hitPt = ray.origin + t * ray.direction;
    if (Math::distSq(center, hitPt) < radiusSquared) {
        tmin = t;
        shade.normal = normal;
        shade.localHitPoint = hitPt;
        shade.materialP = getMaterial();
        return true;
    }
    else return false;
}

bool Disk::shadowIntersect(Ray &ray, double &tmin) {
    double t = dot(center - ray.origin, normal) / dot(ray.direction, normal);
    if (t <= EPSILON) return false;
    
    dvec3 hitPt = ray.origin + t * ray.direction;
    if (Math::distSq(center, hitPt) < radiusSquared) {
        tmin = t;
        return true;
    }
    else return false;
}

void Disk::setSampler(Sampler *sampler_ptr) {
    Geometry::setSampler(sampler_ptr);
    samplerP->mapSamplesToUnitDisk();
    v = normalize(cross(UP_VECTOR, normal));
    u = cross(v, normal);
}

dvec3 Disk::sample() {
    dvec2 samplePt = samplerP->sampleUnitDisk();
    return radius * (samplePt.x * u + samplePt.y * v) + center;
}