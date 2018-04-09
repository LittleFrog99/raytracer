#include "whitted.h"
#include "core/world.h"

vec3 Whitted::traceRay(Ray &ray, int depth) {
    if (depth > worldP->vp.maxDepth)
        return vec3();
    else {
        Shade shade(worldP->intersectObjects(ray));
        if (shade.hasHit) {
            shade.depth = depth;
            shade.ray = ray;
            return worldP->vp.globalIllum ? shade.materialP->globalShade(shade)
                                          : (shade.materialP->shade(shade));
        }
        else return worldP->bgColor;
    }
}
