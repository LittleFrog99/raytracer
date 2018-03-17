#pragma once

#include "utilities.h"

struct Ray {
public:
    dvec3 origin;
    dvec3 direction;

    Ray() {}
    Ray(dvec3 &origin, dvec3 &dir) : origin(origin), direction(dir) {}
};
