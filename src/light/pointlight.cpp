#include "pointlight.h"
#include "core/world.h"

bool PointLight::inShadow(Ray &shadow_ray, Shade &shade) {
    double t;
    double d = distance(shadow_ray.origin, location);

    for (auto objP : shade.world.objects) {
        if (objP->shadowIntersect(shadow_ray, t) && objP->castShadow() && (t < d))
            return true;
    }

    return false;
}