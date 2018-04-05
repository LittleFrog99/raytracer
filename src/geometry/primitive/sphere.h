#pragma once

#include "core/geometry.h"
#include "geometry/bound/boundingbox.h"

class Sphere : public Geometry {
public:
    Sphere() {}
    Sphere(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(dvec3 center, double radius);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    
protected:
    dvec3 center;
    double radius;
    
    virtual BoundingBox calcBoundingBox();
};

