#pragma once

#include "core/geometry.h"

class Rectangle : public Geometry {
public:
    Rectangle(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(dvec3 origin, dvec3 side_a, dvec3 side_b);
    virtual dvec3 sample();
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    virtual dvec3 getNormal(dvec3 &point) { return normal; }
    virtual float calcPdf(Shade &shade) { return 1.0 / area; }

private:
    dvec3 origin = dvec3(-1, 0, -1);
    dvec3 sideA = dvec3(0, 0, 2), sideB = dvec3(2, 0, 0);
    double lenASqrd = 4.0, lenBSqrd = 4.0;
    dvec3 normal = dvec3(0, 1, 0);
    float area = 4.0;
};