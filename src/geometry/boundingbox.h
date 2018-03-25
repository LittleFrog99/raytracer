#pragma once

#include "geometry/bound.h"

class BoundingBox : public Bound {
public:
    BoundingBox() {}
    BoundingBox(dvec3 vertex1, dvec3 vertex2);
    virtual bool intersect(Ray &ray);

private: 
    dvec3 vertMin, vertMax;
};