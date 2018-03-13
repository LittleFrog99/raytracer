#pragma once

#include "core/geometry.h"

class Function2D : public Geometry {
public:
    Function2D(vec3 (*function)(double x, double y)) : function(function) {}
    bool intersect(Ray &ray, double &tmin, Shade &sr);
    virtual ~Function2D() {}
private:
    vec3 (*function)(double x, double y);
};

