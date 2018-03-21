#pragma once

#include "core/geometry.h"

class Rectangle : public Geometry {
public:
    Rectangle(Material *material_ptr) : Geometry(material_ptr) {}
    void setParams(dvec3 origin, dvec3 side_a, dvec3 side_b);
    virtual dvec3 sample();
    virtual float probDensity(Shade &shade);
    virtual dvec3 getNormal(dvec3 &point);

private:
    dvec3 origin;
    dvec3 sideA, sideB;
    dvec3 normal;
    float invArea;
};