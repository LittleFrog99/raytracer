#include "multipleobjects.h"
#include "core/world.h"

vec3 MultipleObjects::traceRay(const Ray &ray) const {
    Shade shade(worldP->intersectWithObjects(ray));
    if (shade.hasHit)
        return shade.color;
    else return worldP->bgColor;
}
