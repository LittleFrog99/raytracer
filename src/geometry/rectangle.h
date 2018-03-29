#pragma once

#include "core/geometry.h"

class Rectangle : public Geometry {
public:
    Rectangle(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(dvec3 origin, dvec3 side_a, dvec3 side_b);
    virtual dvec3 sample();
    virtual float probDensity(Shade &shade);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    virtual dvec3 getNormal(dvec3 &point) { return normal; }

private:
    dvec3 origin;
    dvec3 sideA, sideB;
    double lenASqrd, lenBSqrd;
    dvec3 normal;
    float area, invArea;
};