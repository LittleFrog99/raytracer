#pragma once

#include "core/geometry.h"

class Plane : public Geometry {
public:
    Plane(dvec4 param, Material *material_ptr) : Geometry(material_ptr), param(param) {}
    Plane(dvec3 normal, dvec3 point, Material *material_ptr);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    ~Plane() {}
private:
    dvec4 param;
};

