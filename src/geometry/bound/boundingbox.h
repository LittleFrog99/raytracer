#pragma once

#include "geometry/bound/bound.h"

class BoundingBox : public Bound {
public:
    BoundingBox() {}
    BoundingBox(dvec3 vertex1, dvec3 vertex2);
    virtual bool intersect(Ray &ray);
    virtual bool inside(dvec3 &point);

    dvec3 vertMin, vertMax;
};

class BoxBounded {
public:
    BoxBounded() {}
    virtual BoundingBox getBoundingBox() { return bndBox; }

    inline void createBoundingBox(dvec3 vert1, dvec3 vert2) {
        bndBox = BoundingBox(vert1, vert2);
    }

    inline void createBoundingBox(BoundingBox bnd_box) {
        bndBox = bnd_box;
    }

protected:
    BoundingBox bndBox;
};