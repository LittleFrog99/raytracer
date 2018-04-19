#include "globaltracer.h"
#include "core/world.h"

vec3 GlobalTracer::traceRay(Ray &ray, int depth, double *tmin) {
    if (depth > worldP->vp.maxDepth)
        return Color::BLACK;
    else {
        Shade shade(worldP->intersectObjects(ray));
        if (shade.hasHit) {
            shade.depth = depth;
            shade.ray = ray;
            if (tmin) *tmin = shade.t;
            return worldP->vp.globalIllum ? shade.materialP->globalShade(shade)
                                          : shade.materialP->shade(shade);
        }
        else  {
            if (tmin) *tmin = numeric_limits<double>::max();
            return worldP->bgColor;
        }
    }
}
