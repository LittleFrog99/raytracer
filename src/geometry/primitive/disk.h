#pragma once

#include "core/geometry.h"

class Disk : public Geometry {
public:
    Disk() {}
    Disk(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(dvec3 center, dvec3 normal, double radius);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    virtual void setSampler(Sampler *sampler_ptr);
    virtual dvec3 sample();
    virtual dvec3 getNormal(dvec3 &point) { return normal; }
    virtual float calcPdf(Shade &shade) { return 1.0 / area; }

private:
    dvec3 center;
    dvec3 normal, u, v;
    double radius, radiusSquared;
    double area;
};