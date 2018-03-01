#pragma once

#include "core/tracer.h"

class SingleSphere : public Tracer {
public:
    SingleSphere(World *w_ptr) : Tracer(w_ptr) {}
    virtual vec3 traceRay(const Ray &ray) const;
};
