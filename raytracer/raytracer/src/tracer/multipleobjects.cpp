#include "multipleobjects.h"
#include "core/world.h"

vec3 MultipleObjects::traceRay(Ray &ray, int depth) {
    Shade shade(worldP->intersectObjects(ray));
    if (shade.hasHit)
        return shade.color;
    else return worldP->bgColor;
}
