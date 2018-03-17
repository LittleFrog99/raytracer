#pragma once

#include "core/geometry.h"

class Sphere : public Geometry {
public:
    Sphere() {}
    Sphere(dvec3 center, double radius, Material *material_ptr) : Geometry(material_ptr), center(center), radius(radius) {}
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    
private:
    dvec3 center;
    double radius;
};

