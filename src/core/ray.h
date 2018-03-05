#pragma once

#include "utilities.h"

struct Ray {
public:
    dvec4 origin;
    dvec4 direction;

    Ray() {}
    Ray(const dvec4 &origin, const dvec4 &dir) : origin(origin), direction(dir) {}
    Ray & operator = (const Ray &ray);
};
