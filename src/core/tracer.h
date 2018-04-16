#pragma once

#include "core/ray.h"

class World;

class Tracer {
public:
    Tracer(World *w_ptr) : worldP(w_ptr) {}
    virtual vec3 traceRay(Ray &ray, int depth = 0, double *tmin = nullptr) = 0;
    virtual ~Tracer() {}

protected:
    World *worldP;
};

