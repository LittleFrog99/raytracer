#pragma once

#include "core/ray.h"

class World;

class Tracer {
public:
    Tracer(World *w_ptr) : worldP(w_ptr) {}
    virtual ~Tracer() {} // no need to delete worldP
    virtual vec3 traceRay(const Ray &ray) const;
protected:
    World *worldP;
};

