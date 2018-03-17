#include "ambientoccluder.h"
#include "core/world.h"
#include "core/shade.h"

void AmbientOccluder::setSampler(Sampler *sampler_ptr) {
    if (samplerP) {
        delete samplerP;
        samplerP = nullptr;
    }

    samplerP = sampler_ptr;
    samplerP->mapSamplesToHemisphere(1.0);
}

dvec3 AmbientOccluder::getDirection(Shade &shade) {
    dvec3 sp = samplerP->sampleUnitHemisphere();
    return sp.x * u + sp.y * v + sp.z * w;
}

bool AmbientOccluder::inShadow(Ray &ray, Shade &shade) {
    double t;
    for (int i = 0; i < shade.world.objects.size(); i++) 
        if (shade.world.objects[i]->shadowIntersect(ray, t)) return true;
    return false;
}

vec3 AmbientOccluder::incidRadiosity(Shade &shade) {
    w = shade.normal;
    v = normalize(cross(w, UP_VECTOR));
    u = cross(v, w);

    Random random;
    float occlusion = 0;
    for (int i = 0; i < numOccSamples; i++) {
        Ray shadowRay(shade.hitPoint, getDirection(shade));
        occlusion += inShadow(shadowRay, shade);
    }
    occlusion /= numOccSamples;
    
    return (1 - occlusion * (1 - minAmount)) * intensity * color;
}
