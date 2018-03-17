#pragma once

#include "core/light.h"
#include "core/sampler.h"

class AmbientOccluder : public Light {
public:
    AmbientOccluder(vec3 color = vec3(1.0), float intensity = 1.0, float min_amount = 0.0, 
        Sampler *sampler_ptr = nullptr) 
        : color(color), intensity(intensity), samplerP(sampler_ptr) {}
    virtual dvec3 getDirection(Shade &shade);
    virtual vec3 incidRadiosity(Shade &shade);
    virtual bool inShadow(Ray &shadow_ray, Shade &shade);
    void setSampler(Sampler *sampler_ptr);
    
    inline void setOcclusionSamples(int samples) {
        numOccSamples = samples;
    }

private:
    vec3 color;
    float intensity;
    float minAmount;
    dvec3 u, v, w;
    Sampler *samplerP;
    int numOccSamples = 1;
};