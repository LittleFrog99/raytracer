#pragma once

#include "core/geometry.h"

class OpenCylinder : public Geometry {
public:
    OpenCylinder() {}
    OpenCylinder(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(double bottom_pos, double top_pos, double radius);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    virtual dvec3 getNormal(dvec3 &point);

private:
    double bottom;
    double top;
    double radius;
};