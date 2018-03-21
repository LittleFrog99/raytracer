#include "arealighttracer.h"
#include "core/world.h"

vec3 AreaLightTracer::traceRay(Ray &ray, int depth) {
    Shade shade(worldP->intersectObjects(ray));
    if (shade.hasHit) {
        shade.ray = ray;
        return shade.materialP->areaLightShade(shade);
    }
    else return worldP->bgColor;
}