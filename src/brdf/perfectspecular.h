#pragma once

#include "core/brdf.h"

class PerfectSpecular : public BRDF {
public:
    PerfectSpecular(Sampler *sampler = nullptr) : BRDF(sampler) {}
    virtual vec3 sampleF(Shade &shade, dvec3 &in, dvec3 &out, float *pdf = nullptr);

    inline void setIntensity(float value) {
        intensity = value;
    }

    inline void setColor(vec3 value) {
        color = value;
    }

private:
    float intensity;
    vec3 color;
};