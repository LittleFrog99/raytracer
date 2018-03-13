#pragma once

#include "core/ray.h"

class World;

class Tracer {
public:
    Tracer(World *w_ptr) : worldP(w_ptr) {}
    virtual vec3 traceRay(Ray &ray, int depth = 0) {
        return vec3();
    }
    virtual vec3 traceRay(Ray &ray, float &tmin, int depth) {
        return vec3();
    }

protected:
    World *worldP;
};

