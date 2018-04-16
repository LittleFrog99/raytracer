#pragma once

#include "core/tracer.h"

class GlobalTracer : public Tracer {
public:
    GlobalTracer(World *w_ptr) : Tracer(w_ptr) {}
    virtual vec3 traceRay(Ray &ray, int depth = 0, double *tmin = nullptr);
};