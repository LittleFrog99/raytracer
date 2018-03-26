#pragma once

#include "core/geometry.h"

class Box : public Geometry {
public:
    enum Face { NEG_X, NEG_Y, NEG_Z, POS_X, POS_Y, POS_Z };
    Box() {}
    Box(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(dvec3 vertex1, dvec3 vertex2);
    virtual bool intersect(Ray &ray, double &tmin, Shade &shade);
    virtual bool shadowIntersect(Ray &ray, double &tmin);
    virtual dvec3 getNormal(dvec3 &point);
    dvec3 getNormal(Face face);

private: 
    dvec3 vertMin, vertMax;
};