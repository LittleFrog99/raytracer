#pragma once

#include "core/tracer.h"

class AreaLightTracer : public Tracer {
public:
    AreaLightTracer(World *w_ptr) : Tracer(w_ptr) {}
    virtual vec3 traceRay(Ray &ray, int depth = 0);
};