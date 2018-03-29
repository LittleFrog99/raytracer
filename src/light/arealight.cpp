#include "arealight.h"
#include "core/world.h"

dvec3 AreaLight::getDirection(Shade &shade) {
    samplePt = objectP->sample();
    normal = objectP->getNormal(samplePt);
    out = normalize(samplePt - shade.hitPoint);
    return out;
}

vec3 AreaLight::incidRadiosity(Shade &shade) {
    float nDotD = dot(-normal, out);
    return (nDotD > 0.0) ? objectP->getMaterial()->getEmissiveLight(shade) : vec3();
}

bool AreaLight::inShadow(Ray &ray, Shade &shade) {
    double t;
    double ts = dot(samplePt - ray.origin, ray.direction);

    for (int i = 0; i < shade.world.objects.size(); i++)
        if (shade.world.objects[i]->shadowIntersect(ray, t) && (t < ts))
            return true;
    
    return false;
}

float AreaLight::probDensity(Shade &shade) {
    return objectP->probDensity(shade);
}

float AreaLight::geometryTerm(Shade &shade) {
    float nDotD = dot(-normal, out);
    float dSquared = atten == LINEAR ? distance(samplePt, shade.hitPoint)
                                     : Math::distanceSquared(samplePt, shade.hitPoint);
    return nDotD / dSquared;
}