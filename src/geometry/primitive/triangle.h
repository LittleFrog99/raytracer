#pragma once

#include "core/geometry.h"
#include "geometry/bound/boundingbox.h"

class Triangle : public Geometry {
public:
    Triangle() {}
    Triangle(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(dvec3 vert_a, dvec3 vert_b, dvec3 vert_c);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    virtual dvec3 getNormal(dvec3 &point) { return normal; }

protected:
    dvec3 vertA, vertB, vertC;
    dvec3 normal;
    double area;

    virtual BoundingBox calcBoundingBox();
    bool commonIntersect(Ray &ray, double &t, double &beta, double &gamma);
};