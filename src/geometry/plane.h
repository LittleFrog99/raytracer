#pragma once

#include "core/geometry.h"

class Plane : public Geometry {
public:
    Plane(dvec4 param, vec3 color) : Geometry(color), param(param) {}
    Plane(dvec3 normal, dvec3 point, vec3 color);
    bool intersect(const Ray &ray, double &tmin, Shade &shade) const;
    ~Plane() {}
private:
    dvec4 param;
};

