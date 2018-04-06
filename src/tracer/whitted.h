#pragma once

#include "core/tracer.h"

class Whitted : public Tracer {
public:
    Whitted(World *w_ptr) : Tracer(w_ptr) {}
    virtual vec3 traceRay(Ray &ray, int depth = 0);
};