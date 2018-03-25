#pragma once

#include "core/geometry.h"

class Box : public Geometry {
public:
    Box() {}
    Box(dvec3 vertex1, dvec3 vertex2);
    virtual bool intersect(Ray &ray, double &tmin, Shade &sr);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    virtual dvec3 getNormal(dvec3 &point);

private: 
    dvec3 vertMin, vertMax;
};