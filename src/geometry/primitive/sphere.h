#pragma once

#include "core/geometry.h"

class Sphere : public Geometry {
public:
    Sphere() {}
    Sphere(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(dvec3 center, double radius) {
        this->center = center;
        this->radius = radius;
    }
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    
private:
    dvec3 center;
    double radius;
};

