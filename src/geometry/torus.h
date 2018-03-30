#pragma once

#include "core/geometry.h"
#include "geometry/boundingbox.h"

class Torus : public Geometry {
public:
    Torus() {}
    Torus(Material *mat_ptr) : Geometry(mat_ptr) {}
    void setParams(double swept_r, double tube_r);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    virtual dvec3 getNormal(dvec3 &point);

private:
    double sweptR;
    double tubeR;
    BoundingBox bndBox;
};