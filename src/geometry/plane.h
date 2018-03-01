#pragma once

#include "core/geometry.h"

class Plane : public Geometry {
public:
    Plane(dvec4 param) : param(param) {}
    Plane(dvec3 normal, dvec4 point, vec3 color);
    bool intersect(const Ray &ray, double &tmin, Shade &shade) const;
    ~Plane() {}
private:
    dvec4 param;
};

