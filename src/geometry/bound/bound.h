#pragma once

#include "core/geometry.h"

class Bound : public Geometry {
public:
    Bound() {}
    virtual bool intersect(Ray &ray) = 0;
    virtual bool inside(dvec3 &point) = 0;
};
