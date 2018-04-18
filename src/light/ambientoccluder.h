#pragma once

#include "core/light.h"
#include "core/sampler.h"

class AmbientOccluder : public Light {
public:
    AmbientOccluder(vec3 color, float intensity, float min_amount);
    virtual dvec3 calcDirection(Shade &shade);
    virtual vec3 incidRadiance(Shade &shade);
    virtual bool inShadow(Ray &shadow_ray, Shade &shade);
    void setSampler(Sampler *sampler_ptr);
    virtual ~AmbientOccluder() {
        if (samplerP) delete samplerP;
    }
    
    inline void setOcclusionSamples(int samples) {
        numOccSamples = samples;
    }

private:
    vec3 color;
    float intensity;
    float minAmount;
    dvec3 u, v, w;
    Sampler *samplerP = nullptr;
    int numOccSamples = 1;
};