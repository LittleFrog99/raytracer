#pragma once

#include "core/geometry.h"

class Bound : public Geometry {
public:
    Bound() {}
    virtual bool intersect(Ray &ray) = 0;
};