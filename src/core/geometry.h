#pragma once

#include "core/shade.h"
#include "core/ray.h"

class Geometry {
public:
    vec3 color;

    Geometry() {}
    Geometry(vec3 color) : color(color) {}
    virtual bool intersect(const Ray &ray, double &tmin, Shade &sr) const;
    virtual ~Geometry() {}

protected:
    static constexpr double EPSILON = 1e-6;
};

