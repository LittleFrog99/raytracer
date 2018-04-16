#pragma once

#include "utilities.h"
#include "core/ray.h"

class Bound {
public:
    virtual bool intersect(Ray &ray) = 0;
    virtual bool inside(dvec3 &point) = 0;
};
