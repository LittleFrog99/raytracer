#pragma once 

#include "geometry/compound/compound.h"
#include "geometry/bound/boundingbox.h"

class BeveledCylinder : public Compound, public BoxBounded {
public:
    BeveledCylinder() {}
    BeveledCylinder(Material *mat_ptr);
    void setParams(double bottom_pos, double top_pos, double radius, double bevel_radius);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
};