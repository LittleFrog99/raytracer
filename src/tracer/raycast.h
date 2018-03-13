#pragma once

#include "core/tracer.h"

class RayCast : public Tracer {
public:
    RayCast(World *world_ptr) : Tracer(world_ptr) {}
    virtual vec3 traceRay(Ray &ray, int depth = 0);
};
