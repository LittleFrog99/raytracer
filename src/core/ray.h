#pragma once

#include "utilities.h"

struct Ray {
public:
    dvec3 origin;
    dvec3 direction;

    Ray() {}
    Ray(const dvec4 &origin, const dvec4 &dir) : origin(origin), direction(dir) {}
};
