#include "singlesphere.h"
#include "core/ray.h"
#include "core/shade.h"
#include "core/world.h"

vec3 SingleSphere::traceRay(const Ray &ray) const {
    Shade shade(*worldP);
    double t;
    if (worldP->sphere.intersect(ray, t, shade))
        return vec3(1.0f, 0.0f, 0.0f);
    else return vec3(0.0f);
}
