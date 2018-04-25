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
            switch (worldP->vp.illum) {
                case NONE: return shade.materialP->shade(shade);
                case PATHTRACING: return shade.materialP->pathShade(shade);
                case PHOTONMAPPING: return shade.materialP->photonShade(shade);
            }
        }
        else  {
            if (tmin) *tmin = numeric_limits<double>::max();
            return worldP->bgColor;
        }
    }
}
