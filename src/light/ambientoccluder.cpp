#include "ambientoccluder.h"
#include "core/world.h"
#include "core/shade.h"
#include "sampler/multijittered.h"

AmbientOccluder::AmbientOccluder(vec3 color, float intensity, float min_amount)
    : color(color), intensity(intensity) 
{
    setSampler(new MultiJittered(DEFAULT_NUM_SAMPLES, DEFAULT_NUM_SETS));
}

void AmbientOccluder::setSampler(Sampler *sampler_ptr) {
    if (samplerP) {
        delete samplerP;
        samplerP = nullptr;
    }
    samplerP = sampler_ptr;
    samplerP->mapSamplesToHemisphere(1.0);
}

dvec3 AmbientOccluder::calcDirection(Shade &shade) {
    dvec3 sp = samplerP->sampleUnitHemisphere();
    return sp.x * u + sp.y * v + sp.z * w;
}

bool AmbientOccluder::inShadow(Ray &ray, Shade &shade) {
    double t;
    for (int i = 0; i < shade.world.objects.size(); i++) 
        if (shade.world.objects[i]->shadowIntersect(ray, t)) return true;
    return false;
}

vec3 AmbientOccluder::incidRadiance(Shade &shade) {
    w = shade.normal;
    v = normalize(cross(w, UP_VECTOR));
    u = cross(v, w);

    vec3 ambColor = intensity * minAmount * color;
    for (int i = 0; i < numOccSamples; i++) {
        Ray shadowRay(shade.hitPoint, calcDirection(shade));
        float nDotIn = dot(w, shadowRay.direction);
        if (!inShadow(shadowRay, shade)) 
            ambColor += intensity * (1 - minAmount) * color * nDotIn / float(numOccSamples);
    }
    return ambColor;
}