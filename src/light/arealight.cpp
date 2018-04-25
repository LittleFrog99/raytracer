#include "arealight.h"
#include "core/world.h"
#include "sampler/multijittered.h"

AreaLight::AreaLight(Geometry *object_ptr) : objectP(object_ptr)
{
    samplerP = new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS);
}

dvec3 AreaLight::calcDirection(Shade &shade) {
    samplePt = objectP->sample();
    normal = objectP->getNormal(samplePt);
    out = normalize(samplePt - shade.hitPoint);
    return out;
}

vec3 AreaLight::incidRadiance(Shade &shade) {
    float nDotD = dot(-normal, out);
    return (nDotD > 0.0) ? objectP->getMaterial()->getEmissiveLight(shade) : Color::BLACK;
}

bool AreaLight::inShadow(Ray &ray, Shade &shade) {
    double t;
    double ts = dot(samplePt - ray.origin, ray.direction);

    for (int i = 0; i < shade.world.objects.size(); i++)
        if (shade.world.objects[i]->shadowIntersect(ray, t) && (t < ts))
            return true;
    
    return false;
}

float AreaLight::probDenFunc(Shade &shade) {
    return objectP->probDenFunc(shade);
}

float AreaLight::geoTerm(Shade &shade) {
    float nDotD = dot(-normal, out);
    float dSquared = Math::distanceSquared(samplePt, shade.hitPoint);
    return nDotD / dSquared;
}