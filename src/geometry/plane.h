#pragma once

#include "core/geometry.h"

class Plane : public Geometry {
public:
    Plane() {}
    Plane(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(dvec4 parameters) { param = parameters; }
    void setParams(dvec3 normal, dvec3 point);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);

private:
    dvec4 param;
};

