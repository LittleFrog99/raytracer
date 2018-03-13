#include "raycast.h"
#include "core/world.h"

vec3 RayCast::traceRay(Ray &ray, int depth) {
    Shade sr = worldP->hitObjects(ray);
    if (sr.hasHit) {
        sr.ray = ray;
        return sr.materialP->shade(sr);
    }
    else return worldP->bgColor;
}